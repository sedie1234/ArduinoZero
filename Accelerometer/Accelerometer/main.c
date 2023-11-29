/*
 * GccApplication2.c
 *
 * Created: 7/3/2023 7:19:17 PM
 * Author : suhtw
 */ 

#pragma GCC target ("thumb")

#include "sam.h"

void GCLK_setup();
void I2C_setup();
void USART_setup();
void LIS2DH_I2C_write(unsigned char, unsigned char);
unsigned char LIS2DH_I2C_read(unsigned char);
void          LIS2DH_I2C_read_multiple_data(unsigned char, unsigned char, unsigned char *);
void print_byte(unsigned char);
void print_string(unsigned char *, unsigned int);
void println_string(unsigned char *, unsigned int);

int main()
{ 
	unsigned char hello_msg[] = {"Hello World!"};
	unsigned char endtest_msg[] = {" -- End of Test --"};		
	unsigned char x_msg[] = {"x = "};
	unsigned char y_msg[] = {"y = "};
	unsigned char z_msg[] = {"z = "};				
	unsigned char return_msg[] = {" "};		
				
	char rx_data;
	_Bool	led, rxc_flag;
	unsigned char reg_data;
	unsigned char buffer[6];
	int i, k;
		
	/* Initialize the SAM system */
    SystemInit();
	
//
// PORT setup for Built-in LED output (PA17)
//
	PORT->Group[0].PINCFG[17].reg = 0x0; // peripheral mux enable = 0
	PORT->Group[0].DIR.reg = 0x1 << 17; // Direction: Output

	GCLK_setup();
	
	USART_setup(); 
	
	I2C_setup();

	reg_data = LIS2DH_I2C_read(0x0f); // read from 0x0f (=WHO_AM_I) register

	//  * temperature sensor configuration
    //    set TEMP_EN1, TEMP_EN0 to b'11 in TEMP_CFG_REG (0x1F)
	LIS2DH_I2C_write(0x23, 0x80); // write 0x80 (BDU) to 0x23 (=CTRL_REG4) register
	LIS2DH_I2C_write(0x1f, 0xc0); // write 0xC0 to 0x1F (=TEMP_CFG_REG) register 


	// * 3-axis accelerometer configuration
	//   ODR = 0101 --> 100Hz, Normal Mode (LPen = 0, HR = 0), X,Y,Z enable
	LIS2DH_I2C_write(0x20, 0x57); // write 0x50 to 0x20 (=CTRL_REG1) register
	
	// Read temperature		
	reg_data = LIS2DH_I2C_read(0x0c); // read from 0x0c (=OUT_TEMP_L) register
	reg_data = LIS2DH_I2C_read(0x0d); // read from 0x0d (=OUT_TEMP_H) register
	
	for (i=0; i<=100; i++) {
		// Read accelerometer
		LIS2DH_I2C_read_multiple_data(0x28, 6, buffer); //
		
		print_string(x_msg, sizeof(x_msg));
		print_byte(buffer[1]); // OUT_X_H
		print_byte(buffer[0]); // OUT_X_L
		
		print_string(y_msg, sizeof(y_msg));
		print_byte(buffer[3]); // OUT_Y_H
		print_byte(buffer[2]); // OUT_Y_L
			
		print_string(z_msg, sizeof(z_msg));
		print_byte(buffer[5]); // OUT_Z_H
		print_byte(buffer[4]); // OUT_Z_L
								
		println_string(return_msg, sizeof(return_msg));
		
		for (k=0; k<1000000; k++) ; // just to spend some time...
	}

	println_string(endtest_msg, sizeof(endtest_msg));

	return (0);
}

void print_string(unsigned char * msg, unsigned int msg_size) {

	unsigned count;

	count = 0;
	
	while (1) {
		if (SERCOM5->USART.INTFLAG.bit.DRE == 1) {
			if (count == msg_size)  break;
			else {
				SERCOM5->USART.DATA.reg	= msg[count];
				count++;
			}
		}
	};
}

void println_string(unsigned char * msg, unsigned int msg_size) {

	unsigned count;

	count = 0;
	
	while (1) {
		if (SERCOM5->USART.INTFLAG.bit.DRE == 1) {
			if (count == msg_size) {
				SERCOM5->USART.DATA.reg	= 10; // Line Feed
				while (!SERCOM5->USART.INTFLAG.bit.DRE); // wait until data register is empty
				SERCOM5->USART.DATA.reg	= 13; // Carriage Return
				break;
				} else {
				SERCOM5->USART.DATA.reg	= msg[count];
				count++;
			}
		}
	};
}

void GCLK_setup() {
	
	// OSC8M
	//SYSCTRL->OSC8M.bit.ENABLE = 0; // Disable
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0;
	//SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable
	
	//
	// * Generator #0 is taking the clock source #6 (OSC8M: 8MHz clock input) as an input
	//
	GCLK->GENCTRL.bit.ID = 0; // Generator #0
	GCLK->GENCTRL.bit.SRC = 6; // OSC8M
	GCLK->GENCTRL.bit.OE = 1 ;  // Output Enable: GCLK_IO
	GCLK->GENCTRL.bit.GENEN = 1; // Generator Enable

	GCLK->GENDIV.bit.ID = 1 ; // Generator #1 to check DIV value in debugger	
	
//
// PORT setup for PA14 (GCLK_IO[0]) to check out clock output using logic analyzer
//
	PORT->Group[0].PINCFG[14].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[7].bit.PMUXE = 0x07; // peripheral function H selected

//
// PORT setup for PA15 (GCLK_IO[1]) to check out clock output using logic analyzer
//
/*
	PORT->Group[0].PINCFG[15].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[7].bit.PMUXO = 0x07; // peripheral function H selected
	
*/	
	
}

void I2C_setup() {

	//
	// PORT setup for PA22 and PA23 (I2C)
	//

	PORT->Group[0].PINCFG[22].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PINCFG[23].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[0].PMUX[11].bit.PMUXE = 0x02; // peripheral function C selected
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x02; // peripheral function C selected

	// Power Manager
	PM->APBCMASK.bit.SERCOM3_ = 1 ; // Clock Enable for I2C			
	
	//
	// * SERCOM3: I2C
	// * Generator #0 is feeding I2C

	GCLK->CLKCTRL.bit.ID = 0x17; // ID #17 (SERCOM3: I2C)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for I2C
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to I2C!
	
	// I2C Setup (Host mode)
	SERCOM3->I2CM.CTRLA.bit.SWRST = 1 ; // software reset
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 0 ; // Disable
	//	SERCOM3->I2CM.CTRLA.bit.LOWTOUTEN = 1 ; // SCL Low Timeout Enable

	SERCOM3->I2CM.CTRLA.bit.MODE = 0x5 ; // Host mode
	//SERCOM3->I2CM.BAUD.bit.BAUD = 0x27 ; // 100KHz SCL (0x27 = d'39)
	SERCOM3->I2CM.BAUD.bit.BAUD = 0x20 ; // 100KHz SCL (0x20 = d'32)
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 1 ; // Enable
	SERCOM3->I2CM.STATUS.bit.BUSSTATE = 1 ; // IDLE state	
}

void USART_setup() {
	
//
// PORT setup for PB22 and PB23 (USART)
//
	PORT->Group[1].PINCFG[22].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[1].PINCFG[23].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[1].PMUX[11].bit.PMUXE = 0x03; // peripheral function D selected	
	PORT->Group[1].PMUX[11].bit.PMUXO = 0x03; // peripheral function D selected	

	// Power Manager
	PM->APBCMASK.bit.SERCOM5_ = 1 ; // Clock Enable (APBC clock) for USART
	
	//
	// * SERCOM5: USART
	// * Generator #0 is feeding USART as well
	//
	GCLK->CLKCTRL.bit.ID = 0x19; // ID #0x19 (SERCOM5: USART): GCLK_SERCOM3_CORE
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!

	GCLK->CLKCTRL.bit.ID = 0x13; // ID #0x13 (SERCOM5: USART): GCLK_SERCOM_SLOW
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!
		
	//
	// USART setup
	//
	SERCOM5->USART.CTRLA.bit.MODE = 1 ; // Internal Clock
	SERCOM5->USART.CTRLA.bit.CMODE = 0 ; // Asynchronous UART
	SERCOM5->USART.CTRLA.bit.RXPO = 3 ; // PAD3
	SERCOM5->USART.CTRLA.bit.TXPO = 1 ; // PAD2
	SERCOM5->USART.CTRLB.bit.CHSIZE = 0 ; // 8-bit data
	SERCOM5->USART.CTRLA.bit.DORD = 1 ; // LSB first
	//SERCOM5->USART.CTRLA.bit.SAMPR = 1 ; //

	SERCOM5->USART.BAUD.reg = 0Xc504 ; // 115,200 bps (baud rate) with 8MHz input clock
	//SERCOM5->USART.BAUD.reg = 0Xe282 ; // 115,200 bps (baud rate) with 16MHz input clock
	//SERCOM5->USART.BAUD.reg = 0Xec57 ; // 115,200 bps (baud rate) with 24MHz input clock
	//SERCOM5->USART.BAUD.reg = 0Xf62b ; // 115,200 bps (baud rate) with 48MHz input clock

	SERCOM5->USART.CTRLB.bit.RXEN = 1 ;
	SERCOM5->USART.CTRLB.bit.TXEN = 1 ;

	SERCOM5->USART.CTRLA.bit.ENABLE = 1;	
	
}

void LIS2DH_I2C_write(unsigned char reg_addr, unsigned char data) {
	
//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x30 ; // LIS2DH address (0x18) + Write (0)
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));
	SERCOM3->I2CM.DATA.bit.DATA = reg_addr ; //
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

//
	SERCOM3->I2CM.DATA.bit.DATA = data ; // 
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));
	
}

unsigned char LIS2DH_I2C_read(unsigned char reg_addr) {
	
//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x30 ; // LIS2DH address (0x18) + Write (0)
	// MB (Host on Bus) is set when a byte is transmitted in Host Write mode.
	// RXNAK (Received Not Acknowledge) indicates whether the last address or data packet sent was acked or not
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

	SERCOM3->I2CM.DATA.bit.DATA = reg_addr ; // 
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x31 ; // LIS2DH address (0x18) + Read (1)
	// SB (Client on Bus) is set when a byte is successfully received in Host Read mode
	while (SERCOM3->I2CM.INTFLAG.bit.SB != 1); // Check out SB (Client on Bus) flag
	
	SERCOM3->I2CM.CTRLB.bit.ACKACT = 1 ; // Send NACK
	SERCOM3->I2CM.CTRLB.bit.CMD = 3 ; // Execute ACK succeeded by STOP condition	
	return (SERCOM3->I2CM.DATA.bit.DATA); //  
}

void LIS2DH_I2C_read_multiple_data(unsigned char reg_addr, unsigned char size, unsigned char * buffer) {
	
	//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x30 ; // LIS2DH address (0x18) + Write (0)
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

	SERCOM3->I2CM.DATA.bit.DATA = reg_addr | 0x80; // MSB in register address should be '1'
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

	//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x31 ; // LIS2DH address (0x18) + Read (1)

	while (size != 0 ) {

		while (SERCOM3->I2CM.INTFLAG.bit.SB != 1); // Check out SB (Client on Bus) flag: a byte is successfully received!
		
		*buffer = SERCOM3->I2CM.DATA.reg;
		buffer++;
	
		if (size == 1) {
			SERCOM3->I2CM.CTRLB.bit.ACKACT = 1 ; // Send NACK
			SERCOM3->I2CM.CTRLB.bit.CMD = 3 ; // Execute ACK succeeded by STOP condition
		}
		else {	
			SERCOM3->I2CM.CTRLB.bit.ACKACT = 0; // Send ACK
			SERCOM3->I2CM.CTRLB.bit.CMD = 2 ; // Execute ACK
	
		}
		
		size--;
	};
}

void print_byte(unsigned char data) {

	unsigned char conv_data;

	while (SERCOM5->USART.INTFLAG.bit.DRE != 1) ; // DRE (Data Register Empty) is set when DATA is empty

	conv_data = (data & 0xF0) >> 4 ; // take out the upper 4-bit
	if (conv_data > 9) conv_data += 55 ; // A, B, ... F
	else               conv_data += 48 ; // 0, 1, ... 9
	
	SERCOM5->USART.DATA.reg	= conv_data ; // transmit the first number (ASCII code)

//	
	while (SERCOM5->USART.INTFLAG.bit.DRE != 1) ;

	conv_data = (data & 0x0F) ;  // take out the lower 4-bit
	if (conv_data > 9) conv_data += 55 ; // A, B, ... F
	else               conv_data += 48 ; // 0, 1, ... 9
	
	SERCOM5->USART.DATA.reg	= conv_data ; // transmit the second number (ASCII code)

	while (SERCOM5->USART.INTFLAG.bit.DRE != 1) ;

	SERCOM5->USART.DATA.reg	= 0x20 ; // transmit Space (ASCII code)
	
};
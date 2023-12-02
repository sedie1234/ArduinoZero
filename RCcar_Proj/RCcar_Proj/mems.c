/*
 * mems.c
 *
 * Created: 2023-12-02 오전 9:28:43
 *  Author: hwan
 */ 
#ifndef MEMS_C
#define MEMS_C

#include "sam.h"

void LIS2DH_I2C_write(unsigned char reg_addr, unsigned char data) {
	

	SERCOM3->I2CM.ADDR.bit.ADDR = 0x30 ; // LIS2DH address (0x18) + Write (0)
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));
	SERCOM3->I2CM.DATA.bit.DATA = reg_addr ; //
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));


	SERCOM3->I2CM.DATA.bit.DATA = data ; // 
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));
	
}

unsigned char LIS2DH_I2C_read(unsigned char reg_addr) {
	
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x30 ; // LIS2DH address (0x18) + Write (0)
	// MB (Host on Bus) is set when a byte is transmitted in Host Write mode.
	// RXNAK (Received Not Acknowledge) indicates whether the last address or data packet sent was acked or not
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

	SERCOM3->I2CM.DATA.bit.DATA = reg_addr ; // 
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));


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

void bytes_to_ints(unsigned char * buffer, short * x_mg, short * y_mg, short * z_mg) {

	short x, y, z ;

	x = (short) ((buffer[1] << 8) | buffer[0]) >> 6; // 10-bit data
	y = (short) ((buffer[3] << 8) | buffer[2]) >> 6; // 10-bit data
	z = (short) ((buffer[5] << 8) | buffer[4]) >> 6; // 10-bit data
	
	*x_mg = x << 2 ; // in normal mode: +-2g, 4mg/digit
	*y_mg = y << 2 ; // in normal mode: +-2g, 4mg/digit
	*z_mg = z << 2 ; // in normal mode: +-2g, 4mg/digit		
}

#endif
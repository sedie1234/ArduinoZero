#ifndef MEMS_H
#define MEMS_H

void LIS2DH_I2C_write(unsigned char reg_addr, unsigned char data);
unsigned char LIS2DH_I2C_read(unsigned char reg_addr);
void LIS2DH_I2C_read_multiple_data(unsigned char reg_addr, unsigned char size, unsigned char * buffer);
void bytes_to_ints(unsigned char * buffer, short * x_mg, short * y_mg, short * z_mg);
#endif
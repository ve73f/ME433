#ifndef I2C_H__    /* Guard against multiple inclusion */
#define I2C_H__

void i2c_write(uint8_t state);
unsigned char i2c_read(void);

#endif // I2C_H__
#include <xc.h>
#include "i2c_master_noint.h"

void i2c_write(short data){
    i2c_master_start();
    i2c_master_send(0x40);
    i2c_master_send(0x0A);
    i2c_master_send(data);
    i2c_master_stop();
}

unsigned char i2c_read(void){
    unsigned char data = 0;
    i2c_master_start();
    i2c_master_send(0x40);
    i2c_master_send(0x09);
    i2c_master_restart();
    i2c_master_send(0x41);
    data = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop;
    
    return data;
}
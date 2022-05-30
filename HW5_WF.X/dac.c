#include <proc/p32mx170f256b.h>
#include <xc.h>           // processor SFR definitions
# include "dac.h"
# include "spi.h"

void write_dac(char a_or_b, unsigned char data){
    
    unsigned short s = 0;
    
    LATAbits.LATA0 = 0;
    s = a_or_b <<15;
    s = s + (0b11<<12);
    s = s + (data<<4);
    spi_io(s >> 8);
    spi_io(s);
    LATAbits.LATA0 = 1;
    
}
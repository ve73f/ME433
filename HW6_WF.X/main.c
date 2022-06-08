#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include"i2c_master_noint.h"
#include <stdio.h>
#include <cp0defs.h>
// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use fast frc oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // primary osc disabled
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt value
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz fast rc internal oscillator
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 00000000 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = ON // allow multiple reconfigurations
#pragma config IOL1WAY = ON // allow multiple reconfigurations

void setPin(unsigned char address, unsigned char register_ad, unsigned char value, unsigned char pin);
unsigned char readPin(unsigned char address, unsigned char register_ad, unsigned char pin);

int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

     // setup green led to indicate if i2c is stuck
    TRISAbits.TRISA4 = 0; // set A4 as output
//    LATAbits.LATA4 = 0;   // initialize to output 0

    __builtin_enable_interrupts();
    
    i2c_master_setup();
    i2c_master_start();
    i2c_master_send(0x40);
    i2c_master_send(0x00);
    i2c_master_send(0b01111111);
    i2c_master_stop; 
        
    char heartbeat = 0;
    unsigned char button = 1;
    
    while (1) {
        
        if (heartbeat >100) {
            LATAbits.LATA4 = 1;
        }else{
            LATAbits.LATA4 = 0;
        }
        heartbeat ++;
        if (heartbeat > 10000){
            heartbeat = 0;
        }
        
        setPin(0x40, 0x0A, 0x01,7);
        button = readPin(0x40, 0x09, 0);
       if (button == 0){
            setPin(0x40, 0x0A, 0x00, 7);
//            LATAbits.LATA4 = 1;
        }else{
            setPin(0x40, 0x0A, 0x01, 7);
//            LATAbits.LATA4 = 0;
        }

    }
}

void setPin(unsigned char address, unsigned char register_ad, unsigned char value, unsigned char pin){
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(register_ad);
    i2c_master_send(value<<pin);
    i2c_master_stop();
    
}

unsigned char readPin(unsigned char address, unsigned char register_ad, unsigned char pin){
    unsigned char data = 0;
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(register_ad);
    i2c_master_restart();
    i2c_master_send(address+1);
    data = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop;
    
    data = data & (0x01<<pin);
    return data;
}

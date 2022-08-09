/*
 * File:   Maestro.c
 * Author: Jorge Ceron
 *
 * Created on 4 de agosto de 2022, 07:53 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h> 
#include <stdio.h>
#include "spi.h"
#include "oscilador.h"
#include "tmr0.h"
#define _XTAL_FREQ 1000000


/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t CONTM = 0;
uint8_t CONT_TMR0 = 0;
uint8_t i = 0;
uint8_t VALOR_DESC = 0;
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() SPI_master(void){
    if(INTCONbits.T0IF){
        CONT_TMR0++;                        // Incrementar CONT_TMR0 en 1
        if (CONT_TMR0 == 10){               // CONT_TMR0 10 veces = 1 s
            CONTM++;                        // Incrementar CONTM en 1
            CONT_TMR0 = 0;                  // Limpiar CONT_TMR0
        }
        tmr0_reload();
    }
    return;
}


void main(void) {
    int_osc_MHz(1);
    tmr0_init(255);
    tmr0_reload();
    setup(); 
    while(1){
        PORTEbits.RE0 = 0;                    // SS Activo
        __delay_ms(1);  
        spiWrite(CONTM);            // Se envía el valor del contador
        
        if (i == 0){                // Primer valor que devuelve el Slave es desconocido
            VALOR_DESC = spiRead(); // Se lee este primer valor desconocido
            i = 1;                  // Una vez se lee, los demás envíos son correctos
            __delay_ms(1); 
            PORTEbits.RE0 = 1;                // SS Inactivo
        } else {
            PORTB = spiRead();      // Se muestra el valor enviado por el Slave
            __delay_ms(1); 
            PORTEbits.RE0 = 1;                //SS Inactivo
        }
        __delay_ms(100);
    }
    return;
}

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){       
    // Configuración de puertos
    ANSEL = 0b00000000;         // I/O digitales
    ANSELH = 0b00000000;        // I/O digitales
    TRISB = 0b00000000;         // PORTB como salida
    TRISEbits.TRISE0 = 0;       // RE0 como salida para SS
    PORTB = 0b00000000;         // Limpiar PORTB
    PORTE = 0b0001;             // Limpiar PORTE
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    // Configuración de interrupciones
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de perifericos
    return;
}

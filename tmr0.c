/*
 * File: tmr0.c
 * Author: Jorge Cerón
 *
 * Created on 22 de julio de 2022, 11:46 AM
 */

#include <xc.h>
#include <stdint.h>
#include "tmr0.h"


void tmr0_init(uint8_t prescaler){
    OPTION_REGbits.T0CS = 0;            // Reloj de ciclo de instrucción interna
    if(prescaler != 0){                 // Si es distinto a cero, PSA a TMR0
       OPTION_REGbits.PSA = 0; 
    }
    else if (prescaler == 0){           // Si es igual a cero, PSA a WDT
        OPTION_REGbits.PSA = 1;
    }  
    switch(prescaler){
        case 0:                         // PSA a WDT
            OPTION_REGbits.PS = 0b000;
            break;
        case 2:                         // 1:2
            OPTION_REGbits.PS = 0b000;
            break;
        case 4:                         // 1:4
            OPTION_REGbits.PS = 0b001;
            break;
        case 8:                         // 1:8
            OPTION_REGbits.PS = 0b010;
            break;
        case 16:                         // 1:16
            OPTION_REGbits.PS = 0b011;
            break;
        case 32:                         // 1:32
            OPTION_REGbits.PS = 0b100;
            break;
        case 64:                         // 1:64
            OPTION_REGbits.PS = 0b101;
            break;
        case 128:                         // 1:128
            OPTION_REGbits.PS = 0b110;
            break;
        case 255:                         // 1:256
            OPTION_REGbits.PS = 0b111;
            break;
        default:                         // 1:2
            OPTION_REGbits.PS = 0b000;
            break;
    }
    INTCONbits.T0IE = 1;                // Habilitar interrupciones de TMR0
    INTCONbits.T0IF = 0;                // Limpiar la bandera de TMR0
    return;
}

void tmr0_reload(void){
    TMR0 = _tmr0_value;                 // Valor de retraso  
    INTCONbits.T0IF = 0;                // Limpiar la bandera de TMR0
    return;
}
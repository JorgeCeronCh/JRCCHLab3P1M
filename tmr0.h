/* 
 * File: tmr0.h
 * Author: Jorge Cerón
 * Comments: Prototipo de funciones para configuración de prescaler y recargo de 
 * valor de retraso del TMR0 (PIC16F887)
 * Revision history:  
 */
 
#ifndef TMR0_H
#define	TMR0_H

#include <xc.h>
#include <stdint.h>

#define _tmr0_value 158 // Tiempo = 100 mS
/*
 *_tmr0_value = 256 - (T*Fosc)/(4*Ps) 
 *_tmr0_value = 256-(0.1*1*10^6)/(4*256) = 207
 */

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void tmr0_init(uint8_t prescaler);
/*
La variable "prescaler" corresponde al prescaler del TMR0, donde se tiene que:
prescaler == 0 -> PSA a WDT; Sin PS
prescaler == 2 -> PS = 000 = 1:2 (default)
prescaler == 4 -> PS = 001 = 1:4
prescaler == 8 -> PS = 010 = 1:8
prescaler == 16 -> PS = 011 = 1:16
prescaler == 32 -> PS = 100 = 1:32
prescaler == 64 -> PS = 101 = 1:64
prescaler == 128 -> PS = 110 = 1:128
prescaler == 255 -> PS = 111 = 1:256
 */

void tmr0_reload(void);
/*
Se encarga de recargar el valor de retraso al TMR0 y de limpiar su respectiva
bandera.
 */
#endif	/* TMR0_H */


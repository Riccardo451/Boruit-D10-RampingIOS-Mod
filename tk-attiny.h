#ifndef TK_ATTINY_H
#define TK_ATTINY_H
/* 
 * IU4IRC Modified for Boruit D10 with different layout: 
 * - PWM for Mosfet Led-> PA2
 * - Switch -> PA6
 * - Red Led -> PA1
 * - Green Led-> PA7
 * - 1kohm resistor to GND -> PA0
 * - Not Connected -> PA3
 *
 * Attiny portability header.
 * This helps abstract away the differences between various attiny MCUs.
 *
 * Copyright (C) 2015 Selene Scriven
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Choose your MCU here, or in the main .c file, or in the build script
//#define ATTINY 13
//#define ATTINY 25

/******************** hardware-specific values **************************/
#if (ATTINY == 13)
    #define F_CPU 4800000UL
    #define EEPSIZE 64
    #define V_REF REFS0
    #define BOGOMIPS 950
#elif (ATTINY == 25)
    // TODO: Use 6.4 MHz instead of 8 MHz?
    #define F_CPU 8000000UL
    #define EEPSIZE 128
    #define V_REF REFS1
    #define BOGOMIPS 2000
#elif (ATTINY == 85)
	// TODO: Use 6.4 MHz instead of 8 MHz?
	#define F_CPU 8000000UL
	#define EEPSIZE 512
	#define V_REF REFS1
	#define BOGOMIPS 2000
#elif (ATTINY == 412)
	#define F_CPU 3333333UL
	#define EEPSIZE 128
	#define V_REF REFS1
	#define BOGOMIPS 833
#elif (ATTINY == 416)
	#define F_CPU 3333333UL
	#define EEPSIZE 128
	#define V_REF REFS1
	#define BOGOMIPS 833
#elif (ATTINY == 817)
	#define F_CPU 3333333UL
	#define EEPSIZE 128
	#define V_REF REFS1
	#define BOGOMIPS 833
#else
    Hey, you need to define ATTINY.
#endif


// ****
// Note: you also need to set some pin settings at the top of "int main(void)"!!!
// ****


#define SINGLE_CHANNEL // comment out for dual-channel

#if (ATTINY == 412)
	#define PWM_PIN 2 // PA2, that is
	#define PWM_LVL TCA0.SINGLE.CMP2  // PA2 = WO2

	#ifndef SINGLE_CHANNEL
	#define ALT_PWM_LVL TCA0.SINGLE.CMP0
	#endif

	#define SWITCH_PIN 6  // PA6, that is
	#define SWITCH_PIN_LEVEL (VPORTA.IN & (1 << SWITCH_PIN))
	#define INTERRUPT_VECTOR PORTA_PORT_vect
	
	#define PCINT_on() PORTA.PIN6CTRL |= PORT_ISC_BOTHEDGES_gc
	#define PCINT_off() PORTA.PIN6CTRL &= 0b11111000

#elif ((ATTINY == 416) || (ATTINY == 817))
	#define PWM_PIN 0
	#define PWM_LVL TCA0.SINGLE.CMP0  // change this depending on pin used
	
	#ifndef SINGLE_CHANNEL
	#define ALT_PWM_LVL TCA0.SINGLE.CMP1  // change this depending on pin used
	#endif

	#define SWITCH_PIN 4  // PB4, that is
	#define SWITCH_PIN_LEVEL (VPORTB.IN & (1 << SWITCH_PIN))
	#define INTERRUPT_VECTOR PORTB_PORT_vect
	
	#define PCINT_on() PORTB.PIN4CTRL |= PORT_ISC_BOTHEDGES_gc
	#define PCINT_off() PORTB.PIN4CTRL &= 0b11111000

#else


	/******************** I/O pin and register layout ************************/
	/*
	 *           ----
	 *   Reset -|1  8|- VCC
	 *     OTC -|2  7|- Voltage ADC
	 *  Star 3 -|3  6|- PWM (FET)
	 *     GND -|4  5|- PWM (1x7135)
	 *           ----
	 */
	#define SWITCH_PIN PB3      // Star 4,  MCU pin #2 - pin the switch is connected to
	#define SWITCH_PIN_LEVEL (PINB & (1 << SWITCH_PIN))
	#define INTERRUPT_VECTOR PCINT0_vect

	#define ADCMUX_TEMP 0b10001111  // ADCMUX register setup to read temperature
	#define ADCMUX_VCC  0b00001100  // ADCMUX register setup to read Vbg referenced to Vcc

	#define PWM_PIN     PB1     // pin 6, FET PWM
	#define PWM_LVL     OCR0B   // OCR0B is the output compare register for PB1
	
	#ifndef SINGLE_CHANNEL
	#define ALT_PWM_PIN PB0     // pin 5, 1x7135 PWM
	#define ALT_PWM_LVL OCR0A   // OCR0A is the output compare register for PB0
	#endif
	
	#define PHASE 0xA1          // phase-correct PWM both channels
	//#define FAST 0xA3           // fast PWM both channels  // currently not being used
	
	#define PCINT_on()  GIMSK |= (1 << PCIE)
	#define PCINT_off() GIMSK &= ~(1 << PCIE)

#endif

#endif  // TK_ATTINY_H
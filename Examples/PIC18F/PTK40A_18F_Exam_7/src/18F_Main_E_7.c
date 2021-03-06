/*******************************************************************************
* This is the main program for Example 7 using PTK40A PIC Training Board.
*
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
* Remember to check the Jumper configuration file, also compile and load into PTK40A
*******************************************************************************/
// PTK40A with PIC18F4550 or PIC18F4520
// This template C file should be compiled with:
// MPLAB X IDE v1.70
// XC8 C Compiler for the 8-bit PIC Family
// XC8 Compiler operates in LITE Mode with v1.12
// Last Updated date: 10 June 2014
// Please email support@cytron.com.my if you have any problem


#include <xc.h>         //include the PIC18F model of PIC header file
#include "system.h"	//include system header file for PTK40A
#include "adc.h"		//include adc functions header file
#include "seg7.h"



/*******************************************************************************
* DEVICE CONFIGURATION WORDS                                                   *
*******************************************************************************/
#if defined (__18F4520) 		//if this test file is compiled for PIC18F4520
	//Configuration bits for PIC18F4520
	#pragma config OSC = HS		// Maximum Fosc is 40MHz, so if 20MHz external crystal is used, need to select HS
	//#pragma config OSC = HSPLL	// Maximum Fosc is 40MHz, so HSPLL is select, please change external crystal to 10MHz
	//#pragma config OSC = INTIO67	// Using internal 8MHz Oscillator, RA6 and RA7 as normal IO pin
	#pragma config FCMEN = ON 	// Fail-Safe Clock Monitor Enable bit
	#pragma config IESO = ON		// Internal/External Oscillator Switchover Enable bit
	#pragma config PWRT = ON		// Power Up Timer Enable bit 
	#pragma config BOREN = OFF 	// Brown-out Reset Enable bits
	#pragma config WDT = OFF 		// Watch-Dog Timer Enable bit
	#pragma config CCP2MX = PORTC	//CCP2 at RC1
	#pragma config PBADEN = ON	// PORTB analog pin as analog input after Reset	
	#pragma config LPT1OSC = OFF	// Timer 1 configured for high power operation 
	#pragma config MCLRE = ON		// MCLR pin enable, RE3 input disabled
	#pragma config STVREN = ON	//Stack overflow will cause Reset
	#pragma config LVP = OFF 		// Low voltage programming disable
	#pragma config XINST = OFF		// Instruction set extension and Indexed Addressing mode enabled 
	#pragma config DEBUG = OFF	// Debug disable, RB6 and RB7 as normal IO

#elif defined (__18F4550) 	//if this test file is compiled for PIC18F4550
	//Configuration bits for PIC18F4550
	//#pragma config FOSC = HS		// Maximum Fosc is 48MHz, so if 20MHz external crystal is used, need to select HS
	#pragma config FOSC = HSPLL_HS		// Maximum Fosc is 48MHz, so HSPLL is select, 20MHz can be used, but need to select correct CPUDIV
	//#pragma config FOSC = INTOSC_HS	// Using internal Oscillator, HS use for USB
	#pragma config PLLDIV = 5		//20MHz divide by 5 to obtain 4MHz to enter PLL and become 96MHz
	#pragma config CPUDIV = OSC1_PLL2	//Primary Oscillator source is 96MHz/2
	#pragma config USBDIV = 2		//USB peripheral clock source from 96MHz / 2
	#pragma config FCMEN = ON 	// Fail-Safe Clock Monitor Enable bit
	#pragma config IESO = ON		// Internal/External Oscillator Switchover Enable bit
	#pragma config PWRT = ON		// Power Up Timer Enable bit 
	#pragma config BOR = OFF 	// Brown-out Reset Enable bits
	#pragma config WDT = OFF 		// Watch-Dog Timer Enable bit
	#pragma config VREGEN = ON	//Enable USB Voltage Regulator
	#pragma config CCP2MX = ON	//CCP2 at RC1
	#pragma config PBADEN = ON	// PORTB analog pin as analog input after Reset	
	#pragma config LPT1OSC = OFF	// Timer 1 configured for high power operation 
	#pragma config MCLRE = ON		// MCLR pin enable, RE3 input disabled
	#pragma config STVREN = ON	//Stack overflow will cause Reset
	#pragma config LVP = OFF 		// Low voltage programming disable
	#pragma config XINST = OFF		// Instruction set extension and Indexed Addressing mode enabled 			
	#pragma config DEBUG = OFF	// Debug disable, RB6 and RB7 as normal IO
	#pragma config WRTB = ON		//Boot Block Write Protect Enable
#endif
/*******************************************************************************
* Defination of label		                                                  *
*******************************************************************************/


/*******************************************************************************
* PRIVATE FUNCTION PROTOTYPES                                                  *
*******************************************************************************/

void delay_10us(unsigned int ui_microsecond);
void delay_ms(unsigned int ui_millisecond);
void beep(unsigned char uc_count);

/*******************************************************************************
* Global Variables                                                             *
*******************************************************************************/



/*******************************************************************************
* MAIN FUNCTION                                                                *
*******************************************************************************/
int main(void)
{
	unsigned char i = 0;	// for looping purpose
	unsigned int temperature = 0;	// declare a variable to store temperature read from LM35
	// ensure all the hardware port in zero initially
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;

	// Initialize the I/O port direction, this must be configured according to circuit
	// please refer to PTK40A schematic for details
	// TRISX control pin direction, output pin must be configure as '0'
	// while input must be configure as '1'
	TRISA = 0b00010001;
	TRISB = 0b00001111;
	TRISC = 0b10010011;
	TRISD = 0;
	TRISE = 0;

	// Initialize ADC.
	adc_initialize();	//Ensure pin share with analog is being configured to digital correctly

	// Initialize 7 segments
	seg7_initialize();	//Ensure 7 segment is blank at beginning

	beep(2); 	//buzzer sound for twice

	// PTK40A use an ADC pin (RA0) to be analog input.
	// This analog pin is being shared by 3 analog source, LM35 temperature sensor, potentiometer and external sensor
	// Please refer to the PTK40A schematic for the details connection.
	// Please refer to LM35 datasheet for the details of LM35 output. It state 10mV/ degree celcius
	// Please move JP14 to TEMP to connect ADC pin to LM35
	adc_on();	//activate ADC module in PIC

	while(1) 	// create an infinite loop
	{
		temperature = 0;
		for( i = 0 ; i < 10 ; i++)	// accumulate ADC reading from LM35
		{
		temperature = temperature + ui_adc_read();	// read 10-bit ADC value from AN0, which is LM35
		}

		temperature = temperature / 10;	// Get the average value
		temperature = temperature /2 ;		// resolution of 10-bit ADC around 5mV, LM35 is 10mV/degree Celcius,
										// therefore, ADC result need to be divided by 2 to get the temperature
		seg7_com_dis(temperature);		// display the temperature on 7 segment
	}
	while(1) continue;	// infinite loop to prevent PIC from reset if there is no more program
}



/*******************************************************************************
* PUBLIC FUNCTION: delay_10us
*
* PARAMETERS:
* ~ ui_10microsecond
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* delay for for period 10 microsecond base on ui_10microsecond
* based on _XTAL_FREQ value, ui_10microsecond range from 0-65,535.
*******************************************************************************/
void delay_10us(unsigned int ui_10microsecond)
{
	for( ; ui_10microsecond > 0; ui_10microsecond --)
	{		
		__delay_us(10);	//50 instruction cycle with 20MHz oscillator is 10us
	}	
}


/*******************************************************************************
* PUBLIC FUNCTION: delay_ms
*
* PARAMETERS:
* ~ ui_millisecond
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* delay for for period millisecond base on ui_millisecond
* based on _XTAL_FREQ value, ui_millisecond range from 0-65,535.
*******************************************************************************/
void delay_ms(unsigned int ui_millisecond)
{
	for( ; ui_millisecond > 0; ui_millisecond --)
	{
		__delay_ms(1);
	}		
}	


/*******************************************************************************
* PRIVATE FUNCTION: beep
*
* PARAMETERS:
* ~ uc_count	- How many times we want to beep.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Beep for the specified number of times.
*
*******************************************************************************/
void beep(unsigned char uc_count)
{
	while (uc_count-- > 0) 
	{
		BUZZER = 1;	//buzzer is share at RC2 pin, which is PWM pin
		delay_ms(50);
		BUZZER = 0;
		delay_ms(50);
	}
}


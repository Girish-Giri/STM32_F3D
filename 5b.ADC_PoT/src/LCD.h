/*
 * LCD.h
 *
 *  Created on: 11-May-2020 IST 2:00pm
 *      Author: GIRISH
 */

#ifndef LCD_H_
#include <stdio.h>
#include <math.h>
#define LCD_H_

#define LCDD0Pin 0
#define LCDD0Port GPIOD
#define LCDD1Pin 1
#define LCDD1Port GPIOD
#define LCDD2Pin 2
#define LCDD2Port GPIOD
#define LCDD3Pin 3
#define LCDD3Port GPIOD
#define LCDD4Pin 4
#define LCDD4Port GPIOD
#define LCDD5Pin 5
#define LCDD5Port GPIOD
#define LCDD6Pin 6
#define LCDD6Port GPIOD
#define LCDD7Pin 7
#define LCDD7Port GPIOD

#define LCDEnablePin 10
#define LCDEnablePort GPIOC
#define LCDReadWritePin 11
#define LCDReadWritePort GPIOC
#define LCDRegSelPin 12
#define LCDRegSelPort GPIOC

#define TimeDelayBeforeEnable 4000
#define TimeDelayBeforeDisable 8000


void LCDmsdelay(int delay)
{
	volatile int i, j;
	for (i = 0; i < delay; ++i) {
		++j;
	}
}

//set LCD port pins to output with RCC enable
void LCDsetPinandPortToOut(GPIO_TypeDef *port, int pinNo)
{
	if(port == GPIOA)
		{
			RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  //DO NOT FORGET TO |= , IF NOT IT WILL SET AHBENR FOREVER
		}
	if(port == GPIOB)
		{
			RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
		}
	if(port == GPIOC)
		{
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		}
	if(port == GPIOD)
		{
			RCC->AHBENR |= RCC_AHBENR_GPIODEN;
		}
	if(port == GPIOE)
		{
			RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
		}
	if(port == GPIOF)
		{
			RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
		}


	port->MODER &= ~(1 << ((pinNo * 2) + 1));
	port->MODER |= (1 << (pinNo * 2));
}


//calling above function to select port for LCD (use in main)
void LCDInitializePort()
{

	LCDsetPinandPortToOut(LCDD0Port, LCDD0Pin);
	LCDsetPinandPortToOut(LCDD1Port, LCDD1Pin);
	LCDsetPinandPortToOut(LCDD2Port, LCDD2Pin);
	LCDsetPinandPortToOut(LCDD3Port, LCDD3Pin);
	LCDsetPinandPortToOut(LCDD4Port, LCDD4Pin);
	LCDsetPinandPortToOut(LCDD5Port, LCDD5Pin);
	LCDsetPinandPortToOut(LCDD6Port, LCDD6Pin);
	LCDsetPinandPortToOut(LCDD7Port, LCDD7Pin);
	LCDsetPinandPortToOut(LCDEnablePort, LCDEnablePin);
	LCDsetPinandPortToOut(LCDReadWritePort, LCDReadWritePin);
	LCDsetPinandPortToOut(LCDRegSelPort, LCDRegSelPin);

}


// test each bits of byte of char
void LCDSendBitToPortandPin(GPIO_TypeDef *port, int pinNo, uint8_t bitState)
{
	if(bitState)
		{
			port->ODR |= (1 << pinNo);
		}
		else
		{
			port->ODR &= ~(1 << pinNo);
		}
}


//LCD Timing and configure pin setup:

void LCDEnable ()
{
	LCDmsdelay(TimeDelayBeforeEnable);			//delay before enable goes high
	LCDSendBitToPortandPin(LCDEnablePort, LCDEnablePin, 1);
}


void LCDSetToWrite()
{
	LCDSendBitToPortandPin(LCDReadWritePort, LCDReadWritePin, 0);
}

void LCDSetToRead()
{
	LCDSendBitToPortandPin(LCDReadWritePort, LCDReadWritePin, 1);
}

void LCDCmmndMode()
{
	LCDSendBitToPortandPin(LCDRegSelPort, LCDRegSelPin, 0);
}

void LCDCharMode()
{
	LCDSendBitToPortandPin(LCDRegSelPort, LCDRegSelPin, 1);
}

//receive character to check each bits of a byte

void LCDSendByte(char letter)
{
	LCDSendBitToPortandPin(LCDD0Port, LCDD0Pin, letter & 0b00000001);
	LCDSendBitToPortandPin(LCDD1Port, LCDD1Pin, letter & 0b00000010);
	LCDSendBitToPortandPin(LCDD2Port, LCDD2Pin, letter & 0b00000100);
	LCDSendBitToPortandPin(LCDD3Port, LCDD3Pin, letter & 0b00001000);
	LCDSendBitToPortandPin(LCDD4Port, LCDD4Pin, letter & 0b00010000);
	LCDSendBitToPortandPin(LCDD5Port, LCDD5Pin, letter & 0b00100000);
	LCDSendBitToPortandPin(LCDD6Port, LCDD6Pin, letter & 0b01000000);
	LCDSendBitToPortandPin(LCDD7Port, LCDD7Pin, letter & 0b10000000);

	LCDmsdelay(TimeDelayBeforeDisable);					//delay before enable goes Low
	LCDSendBitToPortandPin(LCDEnablePort, LCDEnablePin, 0);
}

//receive character and set LCD modes + run above to output data (use in main)

void LCDSendCharacter (char letter)
{
	LCDSetToWrite();
	LCDCharMode();
	LCDEnable ();
	LCDSendByte(letter);
}

//receive string of characters (use in main)

void LCDSendString (char *string)
{
	while(*string)
	{
		LCDSendCharacter(*string++);
	}
}


//For LCD data sheet SETUP

void LCDSendCommand (char letter)
{
	LCDSetToWrite();
	LCDCmmndMode();
	LCDEnable ();
	LCDSendByte(letter);
}

//LCD refresh setup  (use in main after LCD initialize)

void LCDSetup()
{
	LCDSendCommand (0b00111100);	//00001DLNF** Function Set
	LCDSendCommand (0b00001110);	//Turn on display and cursor
	LCDSendCommand (0b00000110);	//Set mode to increment address by 1 INPUT SET
	LCDSendCommand (0b00000001);	//Screen Clear
}

void LCDNL()
{
	LCDSendCommand (0b11000000);	//Cursor next line
}


void LCDSend2LinesWithDelay(int delay, char *string1, char *string2)
{
	LCDmsdelay(delay);
	LCDSendCommand (0b00000001);	//Screen Clear
	LCDSendString (string1);
	LCDNL();
	LCDSendString (string2);
}



//receive integer with no. of digits

void LCDSendInteger(int IntegerToBeDisplayed, uint8_t MaxLength)
{
	char StringNumber[MaxLength];   //char type array with size MaxLength
	sprintf(StringNumber, "%d ", IntegerToBeDisplayed);
	LCDSendString(StringNumber);
}


//receive float with no. of digits.
//conventional method won't work to display float so typecast (float*100.0 + 0.5) to int
//then get modulo with 100 so remainder would be mantissa. now print the int part and mantissa part
//defined as %d.%d separated by decimal point.

void LCDSendFloat(float FloatToBeDisplayed, uint8_t MaxLength)
{
	char StringNumber[MaxLength];   //char type array with size MaxLength
	sprintf(StringNumber, "%d.%d  ", (int)FloatToBeDisplayed, ((int)(FloatToBeDisplayed * 100.0 + 0.5))%100);
	//snprintf(StringNumber, MaxLength + 1, "%f", FloatToBeDisplayed);
	LCDSendString(StringNumber);
}


//setting lcd cursor location

void LCDSetCursorLocation(uint8_t x, uint8_t y)
{
	for (int j = 0; j < 2; ++j)
	{
		if(y==0 && x<16)
		{
			LCDSendCommand(0b10000000 + x);
		}

		if(y==1 && x>63 && x<80)
		{
			LCDSendCommand(0b10000000 + x);
		}
	}
}


#endif

#include "stm32f3xx.h"
#include "LCD.h"
#include <stdio.h>
#include <math.h>


int main(void)
{
	LCDInitializePort();
	LCDSetup();

	LCDSetCursorLocation(0, 0);
	LCDSendCharacter('C');
	LCDSendCharacter('I');
	LCDSendCharacter('R');
	LCDSendCharacter('C');
	LCDSendCharacter('U');
	LCDSendCharacter('I');
	LCDSendCharacter('T');
	LCDSendCharacter('C');
	LCDSendCharacter('A');
	LCDSendCharacter('R');
	LCDSendCharacter('D');

	LCDSendFloat(3.14, 5);

	//LCDLN();
	LCDSetCursorLocation(66, 1);
	LCDSendString("Newbiehack");
	LCDSendInteger(96, 2);

	while (1)
  	  {
  	  }
  }


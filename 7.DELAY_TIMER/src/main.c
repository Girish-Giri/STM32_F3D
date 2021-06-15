#include "stm32f3xx.h"
#include "LCD.h"
#include "DELAY.h"

int main(void)
{
	LCDInitializePort();
	LCDSetup();
	DelaySet();

  while (1)
  {
	  LCDSetCursorLocation(0, 0);
	  LCDSendString("HEY MAN!");
	  dMs(2000);
	  LCDSetCursorLocation(0, 0);
	  LCDSendString("hey man!");
	  dMs(2000);
  }

  return 0;
}

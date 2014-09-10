#include <includes.h>


void BoardInit (void)
{
	DDRB = 0XFF;
	//PORTB = 0XFF;
	
	ks0108Init(1);
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	//sei();
	UartInit(103, UART_DATA_8_BITS, UART_PARITY_NONE, UART_STOP_ONE_BIT);
	GsmInit();
}


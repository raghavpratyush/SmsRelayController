#include <includes.h>

#define UBRR_VAL(baud_val) \
	(((CPU_CLOCK_HZ)/(16 * (baud_val))) - 1)

static TagUartStruct UartStruct;


void UartInit(INT16U baud_val, UART_DATA_BITS data_bits, UART_PARITY parity_type, UART_STOP_BIT stop_bit)
{
	INT16U ubrrvalue;

	//Bug here macro not working
	ubrrvalue = UBRR_VAL(baud_val);

	UartStruct.structpUartSem = OSSemCreate(1);
	UartStruct.isRxEn = NO;
	/* Set baud rate */
	UBRRH = (INT8U)(baud_val>>8);
	UBRRL = (INT8U)baud_val;
	/* Enable receiver and transmitter with interrupt*/
	UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE) | (1<<TXCIE);
	/* Set frame format */
	UCSRC = (1<<URSEL) | (data_bits<<UCSZ0) | (stop_bit<<USBS) | (parity_type<<UPM0);
	
}

//Send Routine for UART
BOOL UartSendData (INT8U *data, UartCallBackFuncPtr fptrlUartCallBack, void * ucpPtr, BOOL ucvShouldSleep)
{
	INT8U err;
	INT16U SemCount;
	
	if(ucvShouldSleep EQ YES)
	{
		OSSemPend(UartStruct.structpUartSem, 0, &err);	
	}
	else
	{
		SemCount = OSSemAccept(UartStruct.structpUartSem);
		if(!SemCount)
		{
			return NO;
		}
	}
	
	UartStruct.ucpDataPtr = data;
	UartStruct.fptrUartCallBack = fptrlUartCallBack;
	UartStruct.ptrData = ucpPtr;
	UDR = *UartStruct.ucpDataPtr++;
	return YES;
}

BOOL UartRxEnable(INT8U *buffPtr, INT8U sizeBuff, UartRxCallBackFuncPtr fptrlRxCallBack, void *ucpPtr)
{	
	if(buffPtr EQ NULL)
	{
		return NO;
	}
	
	OS_ENTER_CRITICAL();
	UartStruct.isRxEn = YES;
	UartStruct.Rxindex = 0;
	UartStruct.ucpBuffPtr = buffPtr;
	UartStruct.buffsize = sizeBuff;
	UartStruct.fptrUartRxCallBack = fptrlRxCallBack;
	UartStruct.ptrRxdata = ucpPtr;
	OS_EXIT_CRITICAL();
	return YES;
}

BOOL UartRxDisable(void)
{
	OS_ENTER_CRITICAL();
	UartStruct.isRxEn = NO;
	UartStruct.Rxindex = 0;
	UartStruct.fptrUartRxCallBack = NULL;
	UartStruct.ptrRxdata = NULL;
	UartStruct.ucpBuffPtr = NULL;
	OS_EXIT_CRITICAL();
	return YES;
}

ISR(USART_TXC_vect)
{
	if(*UartStruct.ucpDataPtr)
	{
		UDR = *UartStruct.ucpDataPtr++;
	}
	else
	{
		//Callback should be as short as possible
		//and the callback is atomic function
		if(UartStruct.fptrUartCallBack)
			UartStruct.fptrUartCallBack(UartStruct.ptrData);
		OSSemPost(UartStruct.structpUartSem);
	}
}

INT8U test = 0;

ISR(USART_RXC_vect)
{
	INT8U UartRxData;

	UartRxData = UDR;

	if(UartStruct.isRxEn EQ YES)
	{
		if( (UartStruct.ucpBuffPtr) AND (UartStruct.buffsize) )
		{
			UartStruct.ucpBuffPtr[UartStruct.Rxindex] = UartRxData;
			UartStruct.Rxindex++;
			UartStruct.buffsize--;
		}

		if(UartStruct.fptrUartRxCallBack)
			UartStruct.fptrUartRxCallBack(UartStruct.ptrRxdata,UartStruct.Rxindex,(!UartStruct.buffsize));
	}
}

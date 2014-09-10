#ifndef __SYSTEM_UART_HDR__
#define __SYSTEM_UART_HDR__

typedef enum
{
	UART_STOP_ONE_BIT,
	UART_STOP_TWO_BIT
}UART_STOP_BIT;

typedef enum
{
	UART_PARITY_NONE,
	UART_PARITY_RESERVED_DONT_USE,
	UART_PARITY_EVEN,
	UART_PARITY_ODD
}UART_PARITY;

typedef enum
{
	UART_DATA_5_BITS,
	UART_DATA_6_BITS,
	UART_DATA_7_BITS,
	UART_DATA_8_BITS
}UART_DATA_BITS;

typedef void (*UartCallBackFuncPtr) (void * data);
typedef void (*UartRxCallBackFuncPtr) (void *data, INT16U size, BOOL isFull);

typedef struct
{
	// Transmission Parameters
	INT8U *ucpDataPtr;
	UartCallBackFuncPtr fptrUartCallBack;
	void *ptrData;
	OS_EVENT *structpUartSem;

	//Reception Parameters
	//OS_EVENT *structpUartRxSem;
	INT8U *ucpBuffPtr;
	INT8U buffsize;
	INT8U Rxindex;
	UartRxCallBackFuncPtr fptrUartRxCallBack;
	void *ptrRxdata;
	BOOL isRxEn;
	
}TagUartStruct;



void UartInit(INT16U baud, UART_DATA_BITS data_bits, UART_PARITY parity_type, UART_STOP_BIT stop_bit);
BOOL UartSendData (INT8U *data, UartCallBackFuncPtr fptrlUartCallBack, void * ucpPtr, BOOL ucvShouldSleep);
BOOL UartRxEnable(INT8U *buffPtr, INT8U sizeBuff, UartRxCallBackFuncPtr fptrlRxCallBack, void *ucpPtr);
BOOL UartRxDisable(void);

#endif /* __SYSTEM_UART_HDR__  */
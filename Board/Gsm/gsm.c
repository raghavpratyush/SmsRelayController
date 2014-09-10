#include <includes.h>

static OS_STK TaskGsmTmoutStk[GSM_NO_TASK][GSM_TMOUT_STACK_SIZE];
static OS_EVENT *GsmMessageBox, *GsmTmoutSem,*GsmRespVerifyTask, *GsmCmdSem;
//static INT16U GsmCmdTimeout;
static TagAtCmdDetails *GsmAtCmdDetails;
static INT8U GsmAtCmdBuffer[GSM_BUFFER_SIZE];

static void TaskGsmVerifyResponse(void *data)
{
	INT8U err;
	BOOL strRet;
	TagAtCmdResp RetVal;
	
	while(TRUE)
	{
		OSSemPend(GsmRespVerifyTask,0,&err);

		switch(GsmAtCmdDetails->CmdRsp)
		{
			case GSM_CHK_OK:
				RetVal = GSM_RSP_OK;
				OSSemPost(GsmCmdSem);
				OSMboxPost(GsmMessageBox,&RetVal);
				break;

			default:
				break;
		}
	}
}

static void TaskGsmTimeout(void *data)
{
	INT8U err;
	TagAtCmdResp RetVal;

	//while(TRUE);
	while(TRUE)
	{
		OSSemPend(GsmTmoutSem,0,&err);
		OSSemPend(GsmTmoutSem,(OS_TICKS_PER_SEC * GsmAtCmdDetails->uint16CmdTmout),&err);

		if(err EQ OS_TIMEOUT)
		{				
			UartRxDisable();
			RetVal = GSM_RSP_TMOUT;
			OSSemPost(GsmCmdSem);
			OSMboxPost(GsmMessageBox,&RetVal);
		}
		else
		{
			while(TRUE)
			{
				OSSemPend(GsmTmoutSem,(OS_TICKS_PER_SEC/2),&err);
				if(err EQ OS_TIMEOUT)
				{
					
					UartRxDisable();
					OSSemPost(GsmRespVerifyTask);
					break;
				}
			}
		}
	}
}

static void GsmRxIsrCallback(void *data, INT16U size, BOOL isFull)
{
	if(isFull NEQ YES)
	{
		OSSemPost(GsmTmoutSem);
	}
}

static void CmdTxFinishedCallBack(void *data)
{
	UartRxEnable(GsmAtCmdBuffer,GSM_BUFFER_SIZE,GsmRxIsrCallback,NULL);
	OSSemPost(GsmTmoutSem);
}

BOOL SendAtCommand(TagAtCmdDetails *AtCmdDetails, BOOL ucvShouldSleep, TagAtCmdResp *CmdResp)
{
	//BOOL RetVal;
	INT8U err;
	TagAtCmdResp *msg;
	INT16U SemCount;
	
	if(ucvShouldSleep EQ YES)
	{
		OSSemPend(GsmCmdSem, 0, &err);
	}
	else
	{
		SemCount = OSSemAccept(GsmCmdSem);
		if(!SemCount)
		{
			return NO;
		}
	}
	
	GsmAtCmdDetails = AtCmdDetails;
	UartSendData(AtCmdDetails->uint8CmdPtr,CmdTxFinishedCallBack,NULL,YES);

	msg = (TagAtCmdDetails *)OSMboxPend(GsmMessageBox,0,&err);

	*CmdResp = *msg;
	
	return YES;
}

void GsmInit(void)
{
	GsmMessageBox = OSMboxCreate(NULL);
	GsmCmdSem = OSSemCreate(1);
	GsmTmoutSem = OSSemCreate(0);
	GsmRespVerifyTask = OSSemCreate(0);
	OSTaskCreate(TaskGsmTimeout,NULL, (void *)&TaskGsmTmoutStk[GSM_TIMEOUT_TASK][GSM_TMOUT_STACK_SIZE - 1], GSM_TMOUT_TASK_PRIO);
	OSTaskCreate(TaskGsmVerifyResponse,NULL, (void *)&TaskGsmTmoutStk[GSM_VERIFY_RSP_TASK][GSM_TMOUT_STACK_SIZE - 1], GSM_VERIFY_RSP_TASK_PRIO);
}
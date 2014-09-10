
#define GSM_BUFFER_SIZE			10

#define GSM_NO_TASK				2
#define GSM_TIMEOUT_TASK		0
#define GSM_VERIFY_RSP_TASK		1
//All Stack Sizes for GSM
#define GSM_TMOUT_STACK_SIZE	128

//All priorities for GSM
#define GSM_TMOUT_TASK_PRIO			1
#define GSM_VERIFY_RSP_TASK_PRIO	2

typedef enum
{
	GSM_CHK_ESCAPE,
	GSM_CHK_OK,
	GSM_CHK_ARROW,
	GSM_CHK_CMGS
}TagCmdRspType;

typedef enum
{
	GSM_RSP_OK,
	GSM_RSP_TMOUT,
	GSM_RSP_ERR
}TagAtCmdResp;

typedef struct 
{
	INT8U *uint8CmdPtr;
	INT16U uint16CmdTmout;
	TagCmdRspType CmdRsp;
}TagAtCmdDetails;




BOOL SendAtCommand(TagAtCmdDetails *AtCmdDetails, BOOL ucvShouldSleep, TagAtCmdResp *CmdResp);
void GsmInit(void);
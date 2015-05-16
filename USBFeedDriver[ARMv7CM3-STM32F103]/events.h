// events.h
// 10.09.2014
//------------------------------------------
//#include "types.h"
//------------------------------------------
volatile typedef enum
{
	NO_EVENT			=0x00,
	START_EVT			=0x01,
	NEWSEC_EVT			=0x02,
	RFM_EVT				=0x04,
	BLOCK_EVT			=0x08,
	ECHO_EVT			=0x10,
	UPDATE_TIMEOUT_EVT	=0x20,
	UPDATE_START_EVT	=0x40,
	UPDATE_CANCEL_EVT	=0x80,
	FEED_EVT			=0x0100,
	RESET_MCU_NEED_EVT	=0x08000000
	
}EVENT;

extern EVENT event;

// init.h
#include <stm32f10x.h>
#include "pinsdef.h"
//*********************************************
// *** IN Analog ***
#define INxAN	0
// *** IN Float	***
#define	IN0		0x00000004
#define	IN1		0x00000040
#define	IN2		0x00000400
#define	IN3		0x00004000
#define	IN4		0x00040000
#define	IN5		0x00400000
#define	IN6		0x04000000
#define	IN7		0x40000000
#define	IN8		IN0
#define	IN9		IN1
#define	IN10	IN2
#define	IN11	IN3
#define	IN12	IN4
#define	IN13	IN5
#define	IN14	IN6
#define	IN15	IN7
// *** IN Pull UP/DOWN ***
#define	IN0P	0x00000008
#define	IN1P	0x00000080
#define	IN2P	0x00000800
#define	IN3P	0x00008000
#define	IN4P	0x00080000
#define	IN5P	0x00800000
#define	IN6P	0x08000000
#define	IN7P	0x80000000
#define	IN8P	IN0P
#define	IN9P	IN1P
#define	IN10P	IN2P
#define	IN11P	IN3P
#define	IN12P	IN4P
#define	IN13P	IN5P
#define	IN14P	IN6P
#define	IN15P	IN7P
// *** OUT PushPull (50 MHz) ***
#define	OUT0	0x00000003
#define	OUT1	0x00000030
#define	OUT2	0x00000300
#define	OUT3	0x00003000
#define	OUT4	0x00030000
#define	OUT5	0x00300000
#define	OUT6	0x03000000
#define	OUT7	0x30000000
#define	OUT8	OUT0
#define	OUT9	OUT1
#define	OUT10	OUT2
#define	OUT11	OUT3
#define	OUT12	OUT4
#define	OUT13	OUT5
#define	OUT14	OUT6
#define	OUT15	OUT7
// *** OUT Alternate Function PushPull (50 MHz) ***
#define	OUT0AF	0x0000000B
#define	OUT1AF	0x000000B0
#define	OUT2AF	0x00000B00
#define	OUT3AF	0x0000B000
#define	OUT4AF	0x000B0000
#define	OUT5AF	0x00B00000
#define	OUT6AF	0x0B000000
#define	OUT7AF	0xB0000000
#define	OUT8AF	OUT0AF
#define	OUT9AF	OUT1AF
#define	OUT10AF	OUT2AF
#define	OUT11AF	OUT3AF
#define	OUT12AF	OUT4AF
#define	OUT13AF	OUT5AF
#define	OUT14AF	OUT6AF
#define	OUT15AF	OUT7AF
// *** CLEAR FEATURE	***
#define	xx0		0xFFFFFFF0
#define	xx1		0xFFFFFF0F
#define	xx2		0xFFFFF0FF
#define	xx3		0xFFFF0FFF
#define	xx4		0xFFF0FFFF
#define	xx5		0xFF0FFFFF
#define	xx6		0xF0FFFFFF
#define	xx7		0x0FFFFFFF
#define	xx8		xx0
#define	xx9		xx1
#define	xx10	xx2
#define	xx11	xx3
#define	xx12	xx4
#define	xx13	xx5
#define	xx14	xx6
#define	xx15	xx7
//***********************************************
void ConfigPorts(void);
//***********************************************

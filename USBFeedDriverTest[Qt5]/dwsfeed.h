#ifndef DWSFEED_H
#define DWSFEED_H

/*** DWord Strings defines ***/
/* interfaces */
#define dwDEV		0x3E564544	// DEV> DEVice [common] interface
#define dwsMCU		0x3E55434D	// MCU> MicroController Unit [main in device] interface
#define dwsMCU0		0x3055434D
#define dwsMCU1		0x3155434D
#define dwsMCU2		0x3255434D
#define dwsMCU3		0x3355434D
#define dwsCPU		0x3E555043	// CPU> Central Processing Unit [main in device MCU's kernel]
#define dwsRAM		0x3E4D4152	// abstract RAM> [Random-Access Memory] interface of device
#define dwsROM		0x3E4D4F52	// abstract ROM> [Read-Only Memory] interface of device
#define	dwsMEM		0x3E4D454D	// abstract MEM> [memory] interface of device
#define dwsFILE		0x454C4946	// abstract FILE interface of device
#define dwsDATA		0x41544144	// abstract DATA interface of device
#define dwsLOOP		0x504F4F4C	// LOOP-back received messages
#define dwsFEED		0x44454546	// FEED interface [control channel]
/* requests */
#define dwsNAME		0x454D414E	// NAME [any]
#define dwsINFO		0x4F464E49	// INFOrmation
#define dwsVERS		0x53524556	// VERSion
#define dwsMODL		0x4C444F4D	// MODeL
#define dwsTXDY		0x59445854	// TX DelaY
#define dwsPROG		0x474F5250	// PROGramming
#define	dwsBINX		0x584E4942	// BINX data header
#define dwsREPT		0x54504552	// REPeaT
#define dwsREG		0x3E474552	// REGister [mcu/device]
#define dwsPORT		0x54524F50	// PORT [mcu/device]
#define dwsADDR		0x52444441	// ADDRess [mcu/device]
#define dwsSTAT		0x54415453	// STATe / STATus [any]
#define dwsECHO		0x4F484345	// ECHO [for any interface present]
#define dwsALOC		0x434F4C41	// AlLOCate [memory]
#define dwsFREE		0x45455246	// FREE [memory]
#define dwsUSER		0x52455355	// USER [name]
#define dwsAUTH		0x48545541	// AUTHorization [means process phase]
#define dwsPSW		0x3E575350	// PSW> PaSsWord [input password]
#define dwsSESS		0x53534553	// SESSion [for current user]
#define dwsNIRQ		0x5152494E	// InterRupt Query [query state irq pin]
#define dwsTEST		0x54534554	// TEST [any]
#define dwsBOOT		0x544F4F42	// BOOTing / re-BOOTing
/* answers */
#define dwsOk		0x2E2E6B4F	// Ok..
#define dwsFAIL		0x4C494146	// Fail
#define dwsPSWN		0x3F575350	// PSW? - PaSsWorD Need [need password]
#define dwsERROR	0x524F5245	// Error
#define dwsCRC		0x2E435243	// CRC. [CRC error means]
//-----------------------------------------------------------------------------------------
#endif // DWSFEED_H

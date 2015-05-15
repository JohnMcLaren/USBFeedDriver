// main.c		<USBFeedDriver>
// 10.02.2015
//------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
#include "main.h"
#include "rtc.h"
#include "iwdt.h"
#include "usbfeed.h"
#include "events.h"
#include "testascii.h"
//------------------------------------------------------
char	szMCU_NAME[]="STM32F103RBT6";
char	szDEV_NAME[]="Test STM32 device";
char	szMCU_VERS[]="Firmware version 1.0";
char	szUtfTestRus[]="RUS * здесь текст на русском языке в кодировке UTF-8 * RUS";
EVENT	event;
ULONG	lData, NewSec=0;
char	szFileAnswer[64];
ULONG	lFile =0;

FEED_BYTE_ADDR_REQUEST*	pFEED_BYTE_REQ;
FEED_ADDR_REQUEST*		pFEED_REQ;
//==============================================================================================
int main(void)
{
ConfigPorts();
IniRTC();
IniFeed();
IniIWDT();
	
	while(1)
	{
	IWDT;
//------------------------------------------------------
		if(event & START_EVT)
		{
		event =NO_EVENT;
			
		}	
//------------------------------------------------------
		if(event & NEWSEC_EVT)
		{
		event &=(~NEWSEC_EVT);
			
			if(BUTTON)
				SendIntFeed();
			
			NewSec++;
			LED1 ^=1;
		}
//**************************************************************************
		if(event & FEED_EVT)
		{
		event &=(~FEED_EVT);
			
			if(FeedStatus & RX_COMPLETE)
			{
			pFEED_BYTE_REQ =GetFeedData(&lData);	// получаем указатель на данные и их длину
			
				switch(lData)
				{
				/* пустой фрейм */
				case 0:
					WriteFeed("",0);
					break;
				/* 8 bit interface address */
				/* <1-байтовый адрес> ---> чтение 1 байта с указанного адреса */
				case 1:
					//ReadRFMBytes(pFBYTE->bmAddress,pUSB_TX_BUFF,1);
					//WriteFeed(TX_FEED_BUFFER,1);
					break;
				/* <1-байтовый адрес> <--- запись 1 байта по указанному адресу */
				case 2:
					//WriteRFMBytes(pFBYTE->bmAddress,&pFBYTE->bData,1);
					break;
				/* контроль протокола (зарезервировано, не используется) */
				case 3:
					break;
				/* 4-байта (адрес)->чтение DWORD(4 байта), 0 пакет для адресуемого интерфейса */
//					case 4:
//						WriteFeed((BYTE*)*(DWORD*)pFBYTE,4);
//						break;
				/* 31 bit interface address */
				/* (>= 4 байта) чтение/запись произвольного кол-ва байт */
				default:
				pFEED_REQ=(FEED_ADDR_REQUEST*)pFEED_BYTE_REQ;
	//.............................................................................
	/*** Пакет для интерфейса ***/
					switch(pFEED_REQ->bmAddress)
					{
		/*** Интерфейс устройства ***/
					case dwsDEV:
						switch(pFEED_REQ->dwRequest)
						{
							
						case dwsNAME:
							WriteFeed(szDEV_NAME,0);
							break;
						
						case dwsTEST:
							WriteFeed(szASCIITestRus,0);
							break;
						
						default:
							WriteFeed("Unknown DEVICE request",0);
							break;
						}
						break;
		/*** Интерфейс MCU ***/
					case dwsMCU:
						switch(pFEED_REQ->dwRequest)
						{
							
						case dwsNIRQ:
							//SendFeed(2,(BYTE*)&NIRQ,1);
							break;
						
						case dwsVERS:
							WriteFeed(szMCU_VERS,0);
							break;
						
						case dwsNAME:
							WriteFeed(szMCU_NAME,0);
							break;
						
						default:
							WriteFeed("Unknown MCU request",0);
							break;
						}
						break;
		/*** Интерфейс 'памяти' ***/
					case dwsMEM:
						WriteFeed((BYTE*)pFEED_REQ->dwRequest, pFEED_REQ->dwData);
						break;
		/*** Интерфейс 'файлоприёмника' ***/
					case dwsFILE:
						lFile +=(lData - 4); // (-4) -> 4 bytes iface address
					
						if(lData < FEED_DATA_SIZE)
						{
							sprintf(szFileAnswer,"Ok! File received (%u Bytes)", lFile);
							WriteFeed(szFileAnswer,0);
							lFile =0;
						}
						break;
		/*** Эхо ***/
					case dwsLOOP:
						WriteFeedEx((BYTE*)&pFEED_REQ->dwRequest, lData-4, 1000);
						break;
					
					default:
						WriteFeed("Unknown INTERFACE address",0);
					}
				}
				
			ReadFeed(0,0); // освобождение фид буфера/готов для приёма
			}
//............................................................................
			if(FeedStatus & TX_COMPLETE)
			{
				LED2 ^=1;
				WriteFeed(0, 0); // сброс флага TX_COMPLETE
			}
//............................................................................
			if(FeedStatus & FRAME_CRC_ERROR)
			{
				ResetFeedData();
				WriteFeed("FEED frame CRC error!",0);
			}
		}
//****************************************************************************
	}
}
//===============================================================================================

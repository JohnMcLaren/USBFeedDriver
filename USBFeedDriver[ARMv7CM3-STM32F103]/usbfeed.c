// usbfeed.c
// 27.08.2014
//------------------------------------------------------------------------------------
#include	"usbfeed.h"
#include	"events.h"
#include	<stdlib.h>
#include	<string.h>
//------------------------------------------------------------------------------------
/* Address USB device */
BYTE DeviceAddress =0;
/* EndPoints Registers */
volatile DWORD 			USB_EP[N_EP]	__attribute__((at(USB_BASE_ADDR)));
/* Endpoints PMA Memory Buffers */
volatile USB_EP_BUFF	EP_BUFF[N_EP]	__attribute__((at(USB_PMA_ADDR)));
/* Приёмный буфер 0 точки */
USB_SETUP_PACKET SetupPacket;
//------------------------------------------------------------------------------------
/* статус приёма/пердачи фид буфера */
FEED_STATUS	FeedStatus =CLEAR;
/* Массив структур управления передач точек (инициализация) */
USB_EP_DATA EP_DATA[N_EP]=
{	// Точки ДВУсторонние/полнодуплексные! (статическая инициализация)
	{0,EP_CONTROL,8,8,NULL,0,NULL,0,SETUP_PACKET_SIZE,0},		// 0:нулевая/конфигурационная
	{1,EP_INTERRUPT,MAX_INT_SIZE,0,NULL,0,NULL,0,0,0},			// 1:передача/INT
	{2,EP_BULK,MAX_BULK_SIZE,MAX_BULK_SIZE,NULL,0,NULL,0,FEED_BUFFER_SIZE,0}// 2:передача/прием/BULK
};
/* пакет прерывания фида */
USB_CDC_INT_PULSE_PACKET	ipp={0xA1,CDC_NOTIFICATION_SERIAL_STATE,0,0,2,COMM_DSR_EVT,
								0xA1,CDC_NOTIFICATION_SERIAL_STATE,0,0,2,0};
/*===========================================================================================
* Starts the initialization process of feed channel.
===========================================================================================*/
void IniFeed()
{
/* запрет прерываний USB модуля */
NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
	
/* USB ON */
RCC->APB1ENR =RCC_APB1ENR_USBEN;
	
	/* инициализация USB модуля */
	USB_CNTR = USB_CNTR_FRES;                     /* Force USB Reset */
	USB_ISTR = 0;                                 /* Clear Interrupt Status */
	USB_CNTR = USB_CNTR_RESETM;                   /* USB Reset Interrupt Mask */
	
	/* выделение и указание памяти под буфера приёма точек */
	if(EP_DATA[2].pRX_BUFF)
		free(EP_DATA[2].pRX_BUFF);
	
	EP_DATA[2].pRX_BUFF =(BYTE*)malloc(FEED_BUFFER_SIZE);
	EP_DATA[0].pRX_BUFF =(BYTE*)&SetupPacket;
	
/* разрешение прерываний USB модуля */
NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
/*===========================================================================================
* Asynchronous (non blocking)
* Puts the data to the queue transfers feed channel.
* If successful, returns the length of the queue data.
* In all other cases, returns 0.

* Can be used for interrupt an incomplete transmission.
===========================================================================================*/
ULONG WriteFeed(void* pData,ULONG lData)
{
FeedStatus &=TX_CLEAR;

		if(pData && EP_DATA[2].pTX_BUFF)
			return 0; // если предидущяя передача незавершена
		
		if(pData && !lData)lData	=strlen((char*)pData)+1; // строка + '\0'
		EP_DATA[2].pTX_BUFF			=(WORD*)pData;
		EP_DATA[2].lTX				=lData;

		if(!EP_DATA[2].pTX_BUFF)
			return 0; // если прерывание передачи
	
return USB_RW_EP(&EP_DATA[2], OUT); // добавление передачи в очередь
}
/*===========================================================================================
* Synchronous (blocking)
* Puts the data to the queue transfers feed channel and waits for the end of the transfer.
* If successful, returns the length of the transmitted data.
* In case of failure or timeout triggered returns 0.
===========================================================================================*/
ULONG WriteFeedEx(void* pData,ULONG lData, ULONG lTimeOut)// старт передачи
{
FeedStatus &=TX_CLEAR;
lTimeOut *=(100000 / 180); // Period 1 ms * lTimeOut
while(EP_DATA[2].pTX_BUFF && lTimeOut)lTimeOut--;// если предидущяя передача незавершена
	
	if(pData && lTimeOut)
	{
		if(!lData)lData		=strlen((char*)pData)+1;	// строка + '\0'
		EP_DATA[2].pTX_BUFF	=(WORD*)pData;
		EP_DATA[2].lTX		=lData;
		
		// синхронная передача по таймауту
		lData =USB_RW_EP(&EP_DATA[2],OUT); // добавление передачи в очередь
		
		while(!(FeedStatus & TX_COMPLETE) && lTimeOut)
		{
			lTimeOut--; // ожидание окончания передачи
		}
		
		FeedStatus &=TX_CLEAR;
		
		if(lTimeOut)
			return lData; // успешно
	}

return 0; // нет данных для передачи или таймаут
}
/*===========================================================================================
* If <pBuff> is set - copies data of feed-buffer in said buffer but not more than set <lRead>
					and fills next data to feed-buffer from PMA-buffer (if data present).
* If <pBuff> not set - frees feed-buffer and fills next data from PMA-buffer (if data present).
* Returns length readed data from feed-buffer.
===========================================================================================*/
ULONG ReadFeed(BYTE* pBuff, ULONG lRead) // вызов ReadFeed должен ПОЛНОСТЬЮ освободить RX-буфер фида, а значит указание размера буфера разгрузки не актуально
{
BYTE* pRB =EP_DATA[2].pRX_BUFF + 1; // указатель на данные после заголовка фид-фрейма
BYTE* pWB =pBuff;
if(!lRead)lRead =FEED_DATA_SIZE;
	
	if(pBuff)
	{
	/* DWORD copy (если байт для чтения >= 4) */
		for(;(EP_DATA[2].lRX > 5) && (lRead > 3);EP_DATA[2].lRX -=4, lRead -=4, pWB +=4, pRB +=4)
		{
			*(DWORD*)pWB = *(DWORD*)pRB;
		}
	/* BYTE copy (если есть хотябы 1 байт для чтения) */
		for(;(EP_DATA[2].lRX > 2) && (lRead > 0);EP_DATA[2].lRX--, lRead--, pWB++, pRB++)
		{
			*pWB = *pRB;
		}

		if(EP_DATA[2].lRX == 2)	// если остался только заголовок и crc фрейма
			EP_DATA[2].lRX =0;	
	}
	else
	{
		EP_DATA[2].lRX =0;
	}
	
	if(!EP_DATA[2].lRX)
		FeedStatus &=(RX_CRC_CLEAR);
	
	/* вызов USB_RW_EP готовит точку к следующему приёму ИЛИ сообщает о необходимости чтения буфера (FEED_EVT) */
	if(!USB_RW_EP(&EP_DATA[2], IN) && FeedStatus)
		event |=FEED_EVT;
	
return(pWB - pBuff); // длина прочитанных данных
}
/*===========================================================================================
* Returns a pointer to the data of feed-buffer and length in bytes.
===========================================================================================*/
void* GetFeedData(ULONG* lData)
{
	if(lData)
	{
		if((*lData =EP_DATA[2].lRX) > 2)
			*lData -=2;
		else
			*lData =0;
	}

	if(EP_DATA[2].pRX_BUFF)
		return(EP_DATA[2].pRX_BUFF + 1);
	
return NULL;
}
/*===========================================================================================
* Sends interrupt-packet (rather two) to driver serial port.
===========================================================================================*/
void SendIntFeed()	// отправка прерывания
{
	EP_DATA[1].pTX_BUFF 	=(WORD*)&ipp;
	EP_DATA[1].lTX			=sizeof(USB_CDC_INT_PULSE_PACKET);
	
	USB_RW_EP(&EP_DATA[1], OUT);
}
/*===========================================================================================
* Frees feed-buffer and PMA-buffer of USB module.
===========================================================================================*/
void ResetFeedData()
{
	EP_DATA[2].lRX_PMA =MAX_BULK_SIZE; // принудительный сброс PMA буфера
	ReadFeed(0, 0);
}
/*===========================================================================================
* Calculates CRC of feed-frame.
===========================================================================================*/
BYTE GetCRC8(BYTE* pData, ULONG lData, BYTE bCRC8)
{
	while(lData--)bCRC8 +=(*pData++);
	
return (~bCRC8);
}
/*******************************************************************************************
***									USB Descriptors										 ***
********************************************************************************************
Структура расположения дескрипторов в памяти		/пример/

	+USB_DEVICE_DESCRIPTOR
	-USB_CONFIGURATION_DESCRIPTOR
		-USB_INTERFACE_DESCRIPTOR
			+USB_HDR_FUNCTIONAL_DESCRIPTOR
			+USB_CALL_MGMT_DESCRIPTOR
			+USB_ACM_DESCRIPTOR
			+USB_UNION_IFACE_DESCRIPTOR
			+USB_ENDPOINT_DESCRIPTOR 1(IN_INT)
		-USB_INTERFACE_DESCRIPTOR
			+USB_ENDPOINT_DESCRIPTOR 2(BULK_IN)
			+USB_ENDPOINT_DESCRIPTOR 2(BULK_OUT)
	+USB_STRING_DESCRIPTOR 0
	+USB_STRING_DESCRIPTOR 1
	+USB_STRING_DESCRIPTOR 2
**************************************************************/
const	USB_DEVICE_DESCRIPTOR	USB_DEVICE_DESC __attribute__((used,section("usb_descriptor")))=
{
	(BYTE)		sizeof(USB_DEVICE_DESC),	//	bLength
	(BYTE)  	USB_DEVICE_DESC_TYPE,	//	bDescriptorType
	(USHORT)	0x0110,					//	bcdUSB
	(BYTE)		USB_COMM,				//	bDeviceClass
	(BYTE)		0,						//	bDeviceSubClass
	(BYTE)		0,						//	bDeviceProtocol
	(BYTE)		8,						//	bMaxPacketSize0
	(USHORT)	0xAAAA,					//	idVendor
	(USHORT)	0xFEED,					//	idProduct
	(USHORT)	0x2708,					//	bcdDevice
	(BYTE)		1,						//	iManufacturer	- индекс строки
	(BYTE)		2,						//	iProduct		- индекс строки
	(BYTE)		3,						//	iSerialNumber	- индекс строки
	(BYTE)		1						//	bNumConfigurations
};
//	***	CONFIGURATION 1	***
const	USB_CFG_DESCRIPTOR	USB_CFG_DESC __attribute__((used,section("usb_descriptor")))=
{
	(BYTE)  	sizeof(USB_CFG_DESC),		//	bLength
	(BYTE) 		USB_CFG_DESC_TYPE,		//	bDescriptorType
	(USHORT)	TOTAL_SIZEOF_CFG,		//	wTotalLength
	(BYTE) 		2,						//	bNumInterfaces
	(BYTE) 		1,						//	ConfigurationValue
	(BYTE) 		0,						//	iConfiguration  String Index
	(BYTE) 		USB_CFG_BUS_POWERED,	//	bmAttributes
	(BYTE) 		USB_MAX_POWER(100)		//	MaxPower, mA	
};
const	USB_IFACE_DESCRIPTOR	USB_IFACE_DESC __attribute__((used,section("usb_descriptor")))=
{
	(BYTE)		sizeof(USB_IFACE_DESC),	//	bLength	
	(BYTE)		USB_IFACE_DESC_TYPE,	//	bDescriptorType
	(BYTE) 		0,						//	bInterfaceNumber
	(BYTE) 		0,						//	bAlternateSetting
	(BYTE) 		1,						//	bNumEndpoints
	(BYTE) 		USB_COMM,				//	bInterfaceClass
	(BYTE) 		USB_ACM_COMM,			//	bInterfaceSubClass
	(BYTE) 		0x00,					//	bInterfaceProtocol
	(BYTE) 		4						//	iInterface String Index
};
//..............................................................................................
const	USB_HDR_FUNCTIONAL_DESCRIPTOR	USB_HDR_FUNC_DESC __attribute__((used,section("usb_descriptor")))=
{
	(BYTE)		sizeof(USB_HDR_FUNC_DESC),	// bFunctionLength;
	(BYTE) 		CS_INTERFACE,				// bDescriptorType;
	(BYTE) 		CDC_HDR_DESC,				// bDescriptorSubtype;
	(WORD) 		0x0110						// bcdCDC;
};
const	USB_CALL_MGMT_DESCRIPTOR	USB_CALL_MGMT_DESC __attribute__((used,section("usb_descriptor")))=
{
	(BYTE) 		sizeof(USB_CALL_MGMT_DESC),	// bFunctionLength;
	(BYTE) 		CS_INTERFACE,				// bDescriptorType;
	(BYTE) 		CDC_CALL_MGMT_DESC,			// bDescriptorSubtype;
	(BYTE) 		0x00,						// bmCapabilities;
	(BYTE) 		1							// bDataInterface;
};
const	USB_ACM_DESCRIPTOR	USB_ACM_DESC __attribute__((used,section("usb_descriptor")))=
{
	(BYTE) 		sizeof(USB_ACM_DESC),	// bFunctionLength;
	(BYTE) 		CS_INTERFACE,			// bDescriptorType;
	(BYTE) 		CDC_ACM_DESC,			// bDescriptorSubtype;
	(BYTE) 		0x02						// bmCapabilities;
};
const	USB_UNION_IFACE_DESCRIPTOR	USB_UNION_IFACE_DESC __attribute__((used,section("usb_descriptor")))=
{
	(BYTE) 		sizeof(USB_UNION_IFACE_DESC),	// bFunctionLength;
	(BYTE) 		CS_INTERFACE,					// bDescriptorType;
	(BYTE)  	CDC_UNION_DESC,					// bDescriptorSubtype;
	(BYTE) 		0,								// bMasterInterface;
	(BYTE) 		1								// bSlaveInterface0;
};
//..............................................................................................
const	USB_EP_DESCRIPTOR	USB_EP_DESC1 __attribute__((used,section("usb_descriptor")))=
{
	(BYTE)		sizeof(USB_EP_DESCRIPTOR),	//  bLength,	
	(BYTE)		USB_EP_DESC_TYPE,	//	bDescriptorType
	(BYTE)		USB_EP_IN(1),		//	bEndpointAddress (IN:HOST<-Device, OUT:HOST->Device)
	(BYTE)		USB_EP_TYPE_INT,	//	bmAttributes
	(USHORT)	MAX_INT_SIZE,		//	wMaxPacketSize
	(BYTE)		100					//	bInterval, mS
};
//						***	IFACE 2	***
const	USB_IFACE_DESCRIPTOR	USB_IFACE_DESC2 __attribute__((used,section("usb_descriptor")))=
{
	(BYTE)		sizeof(USB_IFACE_DESC2),	//	bLength	
	(BYTE)		USB_IFACE_DESC_TYPE,	//	bDescriptorType
	(BYTE) 		1,						//	bInterfaceNumber
	(BYTE) 		0,						//	bAlternateSetting
	(BYTE) 		2,						//	bNumEndpoints
	(BYTE) 		CDC_DATA_IFACE,			//	bInterfaceClass
	(BYTE) 		0,						//	bInterfaceSubClass
	(BYTE) 		0,						//	bInterfaceProtocol
	(BYTE) 		5						//	iInterface String Index
};
//	***	ENTRY POINTS 2 ***
const	USB_EP_DESCRIPTOR	USB_EP_DESC2[2] __attribute__((used,section("usb_descriptor")))=
{
	(BYTE)		sizeof(USB_EP_DESCRIPTOR),	//  bLength,	
	(BYTE)		USB_EP_DESC_TYPE,	//	bDescriptorType
	(BYTE)		USB_EP_IN(2),		//	bEndpointAddress (IN:HOST<-Device, OUT:HOST->Device)
	(BYTE)		USB_EP_TYPE_BULK,	//	bmAttributes
	(USHORT)	MAX_BULK_SIZE,		//	wMaxPacketSize
	(BYTE)		0					//	bInterval, mS
,
	(BYTE)		sizeof(USB_EP_DESCRIPTOR),	//  bLength,	
	(BYTE)		USB_EP_DESC_TYPE,	//	bDescriptorType
	(BYTE)		USB_EP_OUT(2),		//	bEndpointAddress (IN:HOST<-Device, OUT:HOST->Device)
	(BYTE)		USB_EP_TYPE_BULK,	//	bmAttributes
	(USHORT)	MAX_BULK_SIZE,		//	wMaxPacketSize
	(BYTE)		0					//	bInterval, mS
};
//	***	CONFIGURATION 2	***
//	***	STRINGS	***
_USB_LANG_ID_	(LANG_US)
_USB_STRING_	(wsVendor,	L"SZ electronics corp.")
_USB_STRING_	(wsProd,	L"USB Feed Port")
_USB_STRING_	(wsSN,		L"0123-4567-89")
_USB_STRING_	(wsCDC,		L"CDC Device")
_USB_STRING_	(wsCDCData,	L"CDC Data")

/*******************************************************************************************
***									USB Routines										 ***
*******************************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if(USB_ISTR & USB_ISTR_RESET)
	{
	USB_ISTR &=(~USB_ISTR_RESET);
		/* USB bus reset */
		ResetUSB();
	}
	else	if(USB_ISTR & USB_ISTR_CTR)
			{
			USB_ISTR &=(~USB_ISTR_CTR);
				/* USB success transaction */
				EPRoutine(USB_ISTR & USB_ISTR_EP_ID);
			}
			else	if(USB_ISTR & USB_ISTR_ERR)
					{
					USB_ISTR &=(~USB_ISTR_ERR);
						/* USB Error handling */
					}
					else
					{
					USB_ISTR =0;
						
					}
}
//******************************************************************************************
void ResetUSB()
{
USHORT c=0;
DWORD offsBufEP = sizeof(EP_BUFF);	// смещение области буферов, сразу после дескрипторов
USB_ISTR = 0;								// сброс флагов прерываний
USB_CNTR = (USB_CNTR_CTRM|USB_CNTR_RESETM);	// маска прерываний
USB_BTABLE = 0x00;                          // смещение области дескрипторов КТ
/***	инициализация буферов КТ (всех объявленных)	***/
for(;c<(N_EP);c++)
{
	EP_BUFF[c].ADDR_TX	=offsBufEP;
	EP_BUFF[c].COUNT_TX	=0;								//EP_DATA[c].MAX_TX_EP;
		offsBufEP +=EP_DATA[c].MAX_TX_EP;
	EP_BUFF[c].ADDR_RX	=offsBufEP;
	/* see RM0008_Reference manual - Table 177. Definition of allocated buffer memory */
	EP_BUFF[c].COUNT_RX	=(EP_DATA[c].MAX_RX_EP << 9);	// предел 62 байта!
		offsBufEP +=EP_DATA[c].MAX_RX_EP;
	
	USB_EP[c] =(EP_DATA[c].nEP|EP_DATA[c].TYPE_EP|RX_VALID|TX_VALID);
}
/***	включение девайса	***/
USB_DADDR = USB_DADDR_EF;
}
//******************************************************************************************
void EPRoutine(BYTE nEP)
{
DWORD valEP	=USB_EP[nEP];
	
	if (valEP & USB_EP0R_CTR_RX) 			// если точка чтото приняла
	{
	// сброс флага приема
	USB_EP[nEP] &=(~EP_CTR_RX & EP_MASK);
		
		if (valEP & USB_EP0R_SETUP) 		// если сетап пакет
		{
		EP_DATA[0].lRX =0;
		USB_RW_EP(&EP_DATA[nEP],IN);
		EP_DATA[0].lTX =NULL;				// сразу готовим пустой пакет
			
			if(SetupPacket.bmRequestType.Type == REQUEST_STANDARD)
			{
				switch(SetupPacket.bRequest)
				{
					
				case USB_REQUEST_SET_ADDRESS:
					DeviceAddress =SetupPacket.wValue.L;
					break;
				
				case USB_REQUEST_GET_DESCRIPTOR:
					GetDescriptor();
				
					if(EP_DATA[0].lTX > SetupPacket.wLength)
						EP_DATA[nEP].lTX =SetupPacket.wLength;
					break;
				}
			}
			
		USB_RW_EP(&EP_DATA[0],OUT);
		}
		else												// все остальные типы входящих посылок
		{													
			if(!USB_RW_EP(&EP_DATA[nEP],IN) && FeedStatus)	// для 0 и 1 точки этого события нет
				event |=FEED_EVT; // буфер заполнен/закончился фрейм
		}
	}
	//......................................................................
	if (valEP & USB_EP0R_CTR_TX) 				// если точка чтото передала
	{
	USB_EP[nEP] &=(~EP_CTR_TX & EP_MASK);		// сброс флага передачи
		
		if(!nEP && (SetupPacket.bmRequestType.Dir == 0))
			USB_DADDR =(USB_DADDR_EF|DeviceAddress); // установка адреса USB устройства
	
		if(!USB_RW_EP(&EP_DATA[nEP],OUT) && FeedStatus)
			event |=FEED_EVT;
	}
}
//******************************************************************************************
void GetDescriptor()
{
ULONG	c;
USB_STR_DESCRIPTOR* 	pSTR;
	
	switch (SetupPacket.wValue.H) 
	{
		
	case USB_DEVICE_DESC_TYPE:
		EP_DATA[0].pTX_BUFF	=(WORD*)&USB_DEVICE_DESC;
		EP_DATA[0].lTX		=sizeof(USB_DEVICE_DESC);
		break;
	
	case USB_CFG_DESC_TYPE:
		EP_DATA[0].pTX_BUFF	=(WORD*)&USB_CFG_DESC;
		EP_DATA[0].lTX		=TOTAL_SIZEOF_CFG;//sp->wValue.L;
		break;
	
	case USB_STR_DESC_TYPE:
		pSTR=(USB_STR_DESCRIPTOR*)&wLANGID;
	
		for(c=0;c<SetupPacket.wValue.L;c++)
		{
			pSTR=(USB_STR_DESCRIPTOR*)((BYTE*)pSTR+pSTR->bLength);// ищем строку по индексу
		}
		EP_DATA[0].pTX_BUFF	=(WORD*)pSTR;
		EP_DATA[0].lTX		=pSTR->bLength;
		break;
	}
}
//******************************************************************************************
ULONG USB_RW_EP(USB_EP_DATA* pEP_DATA, BYTE dir)// длина в байтах, направление ОТ точки!
{
ULONG	c, len =0, lRead;
BYTE	bLenFrame =FEED_BUFFER_SIZE;
ULONG	nEP	=pEP_DATA->nEP;
DWORD*	pPMA_TX=(DWORD*)(USB_PMA_ADDR + 2*EP_BUFF[nEP].ADDR_TX);// Указатель на PMA буфер передачи точки
BYTE*	pPMA_RX=(BYTE*)(USB_PMA_ADDR + 2*EP_BUFF[nEP].ADDR_RX);// Указатель на PMA буфер приема точки
BYTE*	pRX;		// указатель на текущую позицию feed-буфера приема
	
	/*** если передача, pData -> pBuf ***/
	if(dir)
	{
		if(pEP_DATA->pTX_BUFF && pEP_DATA->lTX)
		{
			len =pEP_DATA->lTX;
			if(len > pEP_DATA->MAX_TX_EP)len =pEP_DATA->MAX_TX_EP;	
		//....................................................
			for(c=0; c<(len+1)/2; pPMA_TX++,pEP_DATA->pTX_BUFF++,c++)
			{
				*pPMA_TX = *pEP_DATA->pTX_BUFF;	// копия с расlирением WORD->DWORD
			}
			
		pEP_DATA->lTX -=len;
		}
		else 
		{
			pEP_DATA->lTX =0;			// отправка 0 пакета
			pEP_DATA->pTX_BUFF =NULL;
			
			if(nEP) // 1 и 2 точки НЕ отправляют 0 пакет
			{
				if(nEP > 1)
					FeedStatus |= TX_COMPLETE; // 1 точки это не касаеться
				
			return 0;	// окончание передачи (0 пакет не отправляется)
			}
		}
	/* запуск передачи если, COUNT_TX = 0, будет отправлен 0 пакет */
	EP_BUFF[nEP].COUNT_TX = len;
	EP_Status(nEP|0x80, TX_VALID);
	}
	/*** если прием, pBuf -> pData ***/
	else
	{
		if((len	=EP_BUFF[nEP].COUNT_RX & 0x03FF) > pEP_DATA->lRX_PMA)
		{
		pRX =(pEP_DATA->pRX_BUFF + pEP_DATA->lRX); // указатель на текущую позицию буфера приема
		//.................................................................................
			if(nEP)
			{
				len	-= pEP_DATA->lRX_PMA; // (длина принятых данных (в байтах) - длина уже прочитанных (из них))
				pPMA_RX += ((pEP_DATA->lRX_PMA >> 1) << 2) + (pEP_DATA->lRX_PMA & 1); // (lRX_PMA/2)*4 + остаток, поскольку в DWORD'е лежит 2 только байта
				
				if(pEP_DATA->lRX) 		// узнаём длину фрейма если заголовок уже принят
				{
					bLenFrame =pEP_DATA->pRX_BUFF[0];
					if(len > (bLenFrame - pEP_DATA->lRX))len = (bLenFrame - pEP_DATA->lRX);
				}
			}
		//................................................................................
			for(lRead = len;(lRead > 0);pRX += 2, pPMA_RX += 4)
			{
				/* last or first BYTE copy */
				if(lRead == 1 || pEP_DATA->lRX_PMA & 1)
				{
					*pRX = *pPMA_RX;
					pEP_DATA->lRX++;
					pEP_DATA->lRX_PMA++;
					pPMA_RX--;
					pRX--;
					lRead--;
				}
				/* WORD copy */
				else
				{
					*(WORD*)pRX = *(DWORD*)pPMA_RX; // копия с сокращением DWORD->WORD
					pEP_DATA->lRX +=2;
					pEP_DATA->lRX_PMA +=2;
					lRead -=2;
				}
				/* first recv */
				if(nEP && pEP_DATA->lRX < 3)
				{
					bLenFrame =pEP_DATA->pRX_BUFF[0];
					if(lRead > (bLenFrame - pEP_DATA->lRX))lRead = (bLenFrame - pEP_DATA->lRX);
				}
			}
		//.....................................................................................
			if(nEP && bLenFrame)
			{
				if(pEP_DATA->lRX >= bLenFrame)
				{
					if(GetCRC8(pEP_DATA->pRX_BUFF, pEP_DATA->lRX, 0) == 0)
						FeedStatus |= RX_COMPLETE;
					else
						FeedStatus |= FRAME_CRC_ERROR;
					return 0;
				}
			}
			else
				pEP_DATA->lRX =0;	// "тихий" сброс фид-буфера при 0 фрейме
		//...................................................................................
		}
		
	pEP_DATA->lRX_PMA =0;	// PMA-буфер полностью прочитан и готов к следующему приему
	EP_Status(nEP, RX_VALID);
	}
	
return len;
}
//******************************************************************************************
void EP_Status(DWORD EPNum, DWORD stat) 
{
DWORD num, val;
num = EPNum & 0x0F;
val = USB_EP[num];
	
	if (EPNum & 0x80)					/* IN Endpoint */            
		USB_EP[num] = (val ^ (stat & EP_STAT_TX)) & (EP_MASK | EP_STAT_TX);
	else                                /* OUT Endpoint */
		USB_EP[num] = (val ^ (stat & EP_STAT_RX)) & (EP_MASK | EP_STAT_RX);
}
//******************************************************************************************


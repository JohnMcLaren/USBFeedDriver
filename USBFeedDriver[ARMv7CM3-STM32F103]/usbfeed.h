// usbfeed.h
// 03.08.2014
//---------------------------------------------------------------------------------
#include <stm32f10x.h>
#include "types.h"
#include "dwsfeed.h"
/*=================================================================================
* Definitions FEED structures and functions
=================================================================================*/
#define FEED_BUFFER_SIZE	(255)					// полный размер FEED фрейма
#define FEED_DATA_SIZE		(FEED_BUFFER_SIZE - 2)	// размер данных фрейма без хидера и crc

void	IniFeed(void);
ULONG	WriteFeed(void* pData, ULONG lData);
ULONG	WriteFeedEx(void* pData, ULONG lData, ULONG lTimeOut);
ULONG	ReadFeed(BYTE* pBuff, ULONG lRead);
void*	GetFeedData(ULONG* lData);
void	SendIntFeed(void);
void	ResetFeedData(void);
BYTE	GetCRC8(BYTE* pData, ULONG lData,BYTE bCRC8);
//------------------------------------------------------------
volatile typedef enum //:DWORD
{
	CLEAR			=0x00,
	RX_COMPLETE		=0x01,
	TX_COMPLETE		=0x02,
	FRAME_CRC_ERROR	=0x04,
	TX_CLEAR		= ~(TX_COMPLETE),
	RX_CRC_CLEAR	= ~(RX_COMPLETE|FRAME_CRC_ERROR)
	
}FEED_STATUS;

extern	FEED_STATUS FeedStatus;
//---------------------------------------------------------------------------------
#pragma pack(1)
typedef struct _HDR_FEED_ADDR_REQUEST_
{
	DWORD	bmAddress	: 31;
	DWORD	bmWriteOp	: 1;
	DWORD	dwRequest;
	
}HDR_FEED_ADDR_REQUEST;

typedef struct _FEED_ADDR_REQUEST_
{
	DWORD	bmAddress	: 31;
	DWORD	bmWriteOp	: 1;
	DWORD	dwRequest;
	DWORD	dwData;
	
}FEED_ADDR_REQUEST;

typedef	struct _FEED_BYTE_ADDR_REQUEST_
{
	BYTE	bmAddress	: 7;
	BYTE	bmWriteOp	: 1;
	BYTE	bData;
	
}FEED_BYTE_ADDR_REQUEST;
//...............................................
typedef struct _USB_CDC_INT_PULSE_PACKET_	/* Пакет прерывания CDC */
{
	BYTE 		bmRequestType;
	BYTE        bRequest;
	WORD 		wValue;
	WORD 		wIndex;
	WORD		wLength;
	WORD		wNoticeCode;
	BYTE 		bmRequestType0;
	BYTE        bRequest0;
	WORD 		wValue0;
	WORD 		wIndex0;
	WORD		wLength0;
	WORD		wNoticeCode0;
	
}USB_CDC_INT_PULSE_PACKET;

/* Структура конфигурации и планирования передач точки /для каждой своя/ */
typedef struct _USB_EP_DATA_		
{
	WORD		nEP;				// номер точки
	WORD		TYPE_EP;			// тип точки
	WORD		MAX_TX_EP;			// максимальный размер передачи ТОЧКИ
	WORD		MAX_RX_EP;			// максимальный размер приема ТОЧКИ
	WORD*		pTX_BUFF;			// указатель на передаваемые данные
	ULONG		lTX;				// длина передаваемых данных
	BYTE*		pRX_BUFF;			// указатель на начало приемного буфера
	ULONG		lRX;				// длина принятых данных
	ULONG		RX_BUFF_SIZE;		// размер буфера приема
	ULONG		lRX_PMA;			// прочитано из PMA буфера байт
	
}USB_EP_DATA;
#pragma pack()
/*=================================================================================
* Definitions USB structures and functions
=================================================================================*/
#define	N_EP			3	// общее количество точек по РЕГИСТРАМ МК (включая нулевую! - 0,1,2)
#define	MAX_INT_SIZE	10	// размер пакета прерывания (одного)
#define	MAX_BULK_SIZE	62	// размер разового пакета точки
//=================================================================================
void	ResetUSB(void);
void	EPRoutine(BYTE nEP);
void	GetDescriptor(void);
ULONG	USB_RW_EP(USB_EP_DATA* pEP_DATA, BYTE dir);
void	EP_Status(DWORD EPNum, DWORD stat);
//=================================================================================
#define	TOTAL_SIZEOF_CFG	(sizeof(USB_CFG_DESCRIPTOR)+			\
							sizeof(USB_IFACE_DESCRIPTOR)+			\
							sizeof(USB_HDR_FUNCTIONAL_DESCRIPTOR)+	\
							sizeof(USB_CALL_MGMT_DESCRIPTOR)+		\
							sizeof(USB_ACM_DESCRIPTOR)+				\
							sizeof(USB_UNION_IFACE_DESCRIPTOR)+		\
							sizeof(USB_EP_DESCRIPTOR)+				\
							sizeof(USB_IFACE_DESCRIPTOR)+			\
							sizeof(USB_EP_DESCRIPTOR)+				\
							sizeof(USB_EP_DESCRIPTOR))
							
#define SETUP_PACKET_SIZE	sizeof(USB_SETUP_PACKET)
//--------------------------------------------------------------------------------
#define USB_BASE_ADDR   0x40005C00  		/* USB Registers Base Address 		*/
#define USB_PMA_ADDR    0x40006000  		/* USB Packet Memory Area Address 	*/
#define REG(x)  (*((volatile DWORD*)(x)))
//...........................................
/* Common USB Registers */
#define USB_CNTR    REG(USB_BASE_ADDR + 0x40)   /* Control Register */
#define USB_ISTR    REG(USB_BASE_ADDR + 0x44)   /* Interrupt Status Register */
#define USB_FNR     REG(USB_BASE_ADDR + 0x48)   /* Frame Number Register */
#define USB_DADDR   REG(USB_BASE_ADDR + 0x4C)   /* Device Address Register */
#define USB_BTABLE  REG(USB_BASE_ADDR + 0x50)   /* Buffer Table Address Register */
/* EP_TYPE: EndPoint Types */
#define EP_BULK         0x0000      /* BULK EndPoint */
#define EP_CONTROL      0x0200      /* CONTROL EndPoint */
#define EP_ISOCHRONOUS  0x0400      /* ISOCHRONOUS EndPoint */
#define EP_INTERRUPT    0x0600      /* INTERRUPT EndPoint */
/* EP_STAT_TX: TX Status */
#define TX_DISABLE   0x0000      /* Disabled */
#define TX_STALL     0x0010      /* Stalled */
#define TX_NAK       0x0020      /* NAKed */
#define TX_VALID     0x0030      /* Valid */
/* EP_STAT_RX: RX Status */
#define RX_DISABLE   0x0000      /* Disabled */
#define RX_STALL     0x1000      /* Stalled */
#define RX_NAK       0x2000      /* NAKed */
#define RX_VALID     0x3000      /* Valid */
/* EPxREG: EndPoint Registers Bit Definitions */
#define EP_CTR_RX       0x8000      /* Correct RX Transfer */
#define EP_DTOG_RX      0x4000      /* RX Data Toggle */
#define EP_STAT_RX      0x3000      /* RX Status */
#define EP_SETUP        0x0800      /* EndPoint Setup */
#define EP_TYPE         0x0600      /* EndPoint Type */
#define EP_KIND         0x0100      /* EndPoint Kind */
#define EP_CTR_TX       0x0080      /* Correct TX Transfer */
#define EP_DTOG_TX      0x0040      /* TX Data Toggle */
#define EP_STAT_TX      0x0030      /* TX Status */
#define EP_EA           0x000F      /* EndPoint Address */
/* EndPoint Register Mask (No Toggle Fields) */
#define EP_MASK         (EP_CTR_RX|EP_SETUP|EP_TYPE|EP_KIND|EP_CTR_TX|EP_EA)
//********************************************************************************
/* USB Descriptor Types */
#define USB_DEVICE_DESC_TYPE             	1
#define USB_CFG_DESC_TYPE      				2
#define USB_STR_DESC_TYPE             		3
#define USB_IFACE_DESC_TYPE          		4
#define USB_EP_DESC_TYPE           			5
#define USB_DEVICE_QR_DESC_TYPE   			6
#define USB_OSPEED_CFG_DESC_TYPE 			7
#define USB_IFACE_PWR_DESC_TYPE    			8
/* USB Device Classes */
#define USB_RESERVED					0x00
#define USB_AUDIO                	 	0x01
#define USB_COMM        				0x02
#define USB_HID      					0x03
#define USB_MONITOR               		0x04
#define USB_PHYSIC    					0x05
#define USB_POWER                 		0x06
#define USB_PRINTER               		0x07
#define USB_STORAGE               		0x08
#define USB_HUB                   		0x09
#define USB_VENDOR_SPEC       			0xFF
/* Interface Class SubClass Codes */
#define USB_ACM_COMM        			0x02

#define CDC_DATA_IFACE       			0x0A
#define CS_INTERFACE					0x24
#define CS_ENDPOINT						0x25
/*  SubType in Functional Descriptors */
#define CDC_HDR_DESC					0x00
#define CDC_CALL_MGMT_DESC				0x01
#define CDC_ACM_DESC					0x02
#define CDC_UNION_DESC					0x06
/* bmAttributes in Configuration Descriptor */
#define USB_CFG_POWERED_MASK            0xC0
#define USB_CFG_BUS_POWERED             0x80
#define USB_CFG_SELF_POWERED            0xC0
#define USB_CFG_REMOTE_WAKEUP           0x20
/* bMaxPower in Configuration Descriptor */
#define USB_MAX_POWER(mA)			((mA)/2)
/* bEndpointAddress in Endpoint Descriptor */
#define USB_EP_DIR_MASK					0x80
#define USB_EP_OUT(addr)		((addr)|0x00)
#define USB_EP_IN(addr)         ((addr)|0x80)
/* bmAttributes in Endpoint Descriptor */
#define USB_EP_TYPE_MASK				0x03
#define USB_EP_TYPE_CONTROL				0x00
#define USB_EP_TYPE_ISOCHRONOUS			0x01
#define USB_EP_TYPE_BULK				0x02
#define USB_EP_TYPE_INT					0x03
#define USB_EP_SYNC_MASK				0x0C
#define USB_EP_SYNC_NSYNC				0x00
#define USB_EP_SYNC_ASYNC				0x04
#define USB_EP_SYNC_ADAPT				0x08
#define USB_EP_SYNC_SYNC				0x0C
#define USB_EP_USAGE_MASK				0x30
#define USB_EP_USAGE_DATA				0x00
#define USB_EP_USAGE_FBACK				0x10
#define USB_EP_USAGE_IMPL_FBACK			0x20
#define USB_EP_USAGE_RESERVED			0x30
/* bmRequestType.Type */
#define REQUEST_STANDARD           		0
#define REQUEST_CLASS              		1
#define REQUEST_VENDOR             		2
#define REQUEST_RESERVED           		3
/* USB Standard Request Codes */
#define USB_REQUEST_GET_STATUS          0
#define USB_REQUEST_CLEAR_FEATURE       1
#define USB_REQUEST_SET_FEATURE         3
#define USB_REQUEST_SET_ADDRESS         5
#define USB_REQUEST_GET_DESCRIPTOR      6
#define USB_REQUEST_SET_DESCRIPTOR      7
#define USB_REQUEST_GET_CONFIGURATION   8
#define USB_REQUEST_SET_CONFIGURATION   9
#define USB_REQUEST_GET_INTERFACE       10
#define USB_REQUEST_SET_INTERFACE       11
#define USB_REQUEST_SYNC_FRAME          12
//***********************************************************************
#define	IN	0	// в точку <- от хоста
#define	OUT	1	// из точки -> хосту
#define	LANG_US	(WCHAR)0x0409

#pragma pack(1)
typedef struct _USB_DEVICE_DESCRIPTOR_
{
  BYTE  bLength;
  BYTE  bDescriptorType;
  USHORT bcdUSB;
  BYTE  bDeviceClass;
  BYTE  bDeviceSubClass;
  BYTE  bDeviceProtocol;
  BYTE  bMaxPacketSize0;
  USHORT idVendor;
  USHORT idProduct;
  USHORT bcdDevice;
  BYTE  iManufacturer;
  BYTE  iProduct;
  BYTE  iSerialNumber;
  BYTE  bNumConfigurations;
}USB_DEVICE_DESCRIPTOR;

typedef struct _USB_CONFIGURATION_DESCRIPTOR_
{
  BYTE  bLength;
  BYTE  bDescriptorType;
  USHORT wTotalLength;
  BYTE  bNumInterfaces;
  BYTE  bConfigurationValue;
  BYTE  iConfiguration;
  BYTE  bmAttributes;
  BYTE  MaxPower;
}USB_CFG_DESCRIPTOR;

typedef struct _USB_INTERFACE_DESCRIPTOR_
{
  BYTE bLength;
  BYTE bDescriptorType;
  BYTE bInterfaceNumber;
  BYTE bAlternateSetting;
  BYTE bNumEndpoints;
  BYTE bInterfaceClass;
  BYTE bInterfaceSubClass;
  BYTE bInterfaceProtocol;
  BYTE iInterface;
}USB_IFACE_DESCRIPTOR;

typedef struct _USB_HDR_FUNCTIONAL_DESCRIPTOR_
{
	BYTE bFunctionLength;
	BYTE bDescriptorType;
	BYTE bDescriptorSubtype;
	WORD bcdCDC;
}USB_HDR_FUNCTIONAL_DESCRIPTOR;

typedef struct _MGMT_CAPS_
{
    BYTE D0		: 1;
    BYTE D1		: 1;
	BYTE D72	: 6;
}MGMT_CAPS;

typedef struct _USB_CALL_MGMT_DESCRIPTOR_
{
	BYTE bFunctionLength;
	BYTE bDescriptorType;
	BYTE bDescriptorSubtype;
	BYTE bmCapabilities;		// MGMT_CAPS
	BYTE bDataInterface;
}USB_CALL_MGMT_DESCRIPTOR;

typedef struct _ACM_CAPS_
{
    BYTE D0		: 1;
    BYTE D1		: 1;
	BYTE D2		: 1;
    BYTE D3		: 1;
	BYTE D74	: 4;
}ACM_CAPS;

typedef struct _USB_ACM_DESCRIPTOR_
{
	BYTE bFunctionLength;
	BYTE bDescriptorType;
	BYTE bDescriptorSubtype;
	BYTE bmCapabilities;		// ACM_CAPS
}USB_ACM_DESCRIPTOR;

typedef struct _USB_UNION_IFACE_DESCRIPTOR_
{
	BYTE bFunctionLength;
	BYTE bDescriptorType;
	BYTE bDescriptorSubtype;
	BYTE bMasterInterface;
	BYTE bSlaveInterface0;
}USB_UNION_IFACE_DESCRIPTOR;

typedef struct _USB_ENDPOINT_DESCRIPTOR_
{
  BYTE  bLength;
  BYTE  bDescriptorType;
  BYTE  bEndpointAddress;
  BYTE  bmAttributes;
  USHORT wMaxPacketSize;
  BYTE  bInterval;
}USB_EP_DESCRIPTOR;

typedef struct _WORD_BYTE_
{
    BYTE L 		: 8;
    BYTE H		: 8;
}WORD_BYTE;

typedef struct _REQUEST_TYPE_
{
    BYTE Recipient : 5;
    BYTE Type      : 2;
	BYTE Dir       : 1;
}REQUEST_TYPE;

typedef struct _USB_SETUP_PACKET_	/* */
{
	REQUEST_TYPE 	bmRequestType;
	BYTE         	bRequest;
	WORD_BYTE 		wValue;
	WORD_BYTE 		wIndex;
	WORD			wLength;
}USB_SETUP_PACKET;

typedef struct _USB_STRING_DESCRIPTOR_
{
  BYTE		bLength;
  BYTE		bDescriptorType;
// WCHAR bString[1];
}USB_STR_DESCRIPTOR;

typedef struct _USB_CDC_NOTICE_PACKET_	/* */
{
	REQUEST_TYPE 	bmRequestType;
	BYTE         	bRequest;
	WORD_BYTE 		wValue;
	WORD_BYTE 		wIndex;
	WORD			wLength;
	WORD			wNoticeCode;
}USB_CDC_NOTICE_PACKET;

// Структура контроля выделенной памяти под буфера приема/передачи точки в PMA области /для каждой своя/
typedef struct _USB_EP_BUFF_	
{
	DWORD ADDR_TX;
	DWORD COUNT_TX;
	DWORD ADDR_RX;
	DWORD COUNT_RX;
}USB_EP_BUFF;
#pragma pack()

/* Нехитрый макрос позволяющий объявить строковый дескриптор в человеческом виде. */
#define _USB_STRING_(name,ws)							\
const struct	name									\
{														\
	BYTE	bLength;									\
	BYTE	bDescriptorType;							\
	WCHAR 	bString[(sizeof(ws)-2)/2];					\
}														\
name __attribute__((used, section("usb_string")))={sizeof(name),0x03,ws};
//............................
#define _USB_LANG_ID_(lng_id)							\
const struct	wLANGID									\
{														\
	BYTE	bLength;									\
	BYTE	bDescriptorType;							\
	WCHAR 	bString[1];									\
}														\
wLANGID __attribute__((used, section("usb_string")))={0x04,0x03,lng_id};
//=================================================================================
#define CDC_NOTIFICATION_SERIAL_STATE           0x20
// (usbcdc11.pdf, 6.3.5, Table 69)
#define CDC_SERIAL_STATE_OVERRUN                (1 << 6)  // receive data overrun error has occurred
#define CDC_SERIAL_STATE_PARITY                 (1 << 5)  // parity error has occurred
#define CDC_SERIAL_STATE_FRAMING                (1 << 4)  // framing error has occurred
#define CDC_SERIAL_STATE_RING                   (1 << 3)  // state of ring signal detection
#define CDC_SERIAL_STATE_BREAK                  (1 << 2)  // state of break detection
#define CDC_SERIAL_STATE_TX_CARRIER             (1 << 1)  // state of transmission carrier
#define CDC_SERIAL_STATE_RX_CARRIER             (1 << 0)  // state of receiver carrier

#define	COMM_DSR_EVT							CDC_SERIAL_STATE_TX_CARRIER
//=================================================================================

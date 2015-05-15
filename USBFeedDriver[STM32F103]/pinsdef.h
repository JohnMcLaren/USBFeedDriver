// pinsdef.h		(PinsDefinitions)
#include "bitband.h"
//--------------------------------------------
/*** SPI ***/
//#define	NIRQ	DEFBITPPH(GPIOB_IDR,7)
//#define	NSEL	DEFBITPPH(GPIOA_ODR,4)
//#define	SCK		DEFBITPPH(GPIOA_ODR,5)
//#define	SDI		DEFBITPPH(GPIOA_ODR,7) 
//#define	SDO		DEFBITPPH(GPIOA_IDR,6)
//#define	SDN		DEFBITPPH(GPIOA_ODR,15)

#define	LED1				DEFBITPPH(GPIOA_ODR,5)	// выход зеленый светодиод
#define	LED2				DEFBITPPH(GPIOA_ODR,1)	// выход желтый светодиод
#define	USB_PWR_IN			DEFBITPPH(GPIOA_IDR,0)	// вход сигнал включения USB
#define	PULLUP_USB_PWR_IN	DEFBITPPH(GPIOA_ODR,0)	// подтяжка входа 
#define	USB_PWR_OUT			DEFBITPPH(GPIOC_ODR,12)	// выход вкл/выкл транзистора USB
#define	BUTTON				DEFBITPPH(GPIOC_IDR,9)	// кнопка прерывания
//----------------------------------

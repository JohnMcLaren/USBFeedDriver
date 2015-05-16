//init.c
#include "init.h"
//------------------------------------------------------------------------------------------
void SystemInit(void)
{
__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
  /* Set HSION bit */
	RCC->CR |= RCC_CR_HSION;
  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */// 0xF8FF0000
	RCC->CFGR &= ~(RCC_CFGR_MCO|RCC_CFGR_ADCPRE|RCC_CFGR_PPRE2|RCC_CFGR_PPRE1|RCC_CFGR_HPRE|RCC_CFGR_SW);
  /* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= ~(RCC_CR_HSEON|RCC_CR_CSSON|RCC_CR_PLLON);
  /* Reset HSEBYP bit */
	RCC->CR &= ~(RCC_CR_HSEBYP);
  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */// 0xFF80FFFF
	RCC->CFGR &= ~(RCC_CFGR_PLLSRC|RCC_CFGR_PLLXTPRE|RCC_CFGR_PLLMULL|RCC_CFGR_USBPRE);
  /* Disable all interrupts and clear pending bits  *///0x009F0000;
	RCC->CIR = (RCC_CIR_CSSC|RCC_CIR_PLLRDYC|RCC_CIR_HSERDYC|RCC_CIR_HSIRDYC|RCC_CIR_LSERDYC|RCC_CIR_LSIRDYC);
//-------------------------------------------------------------------------------------------
  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /* Configure the Flash Latency cycles and enable prefetch buffer */
//-------------------------------------------------------------------------------------------
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
	HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;
/*
AHB  - HPRE [0000]	(/1)	- HPRE:AHB prescaler
APB2 - PPRE2[000] 	(/1)	- PPRE2:APB high-speed prescaler (APB2)
APB1 - PPRE1[100]	(/2)	- PPRE1:APB low-speed prescaler (APB1)
USB  - USBPRE[1]	(/1.5)	- USBPRE:USB prescaler
ADC  - ADCPRE[10]	(/6)	- ADCPRE:ADC prescaler
*/
    /* AHB */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
    /* APB2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    /* APB1 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
	/* USB */
	//RCC->CFGR |= (uint32_t)RCC_CFGR_USBPRE; // (!!!) x/1
	/* ADC */
	RCC->CFGR |= (uint32_t)RCC_CFGR_ADCPRE_DIV6;
	
    /***  PLL configuration: PLLCLK = HSE * 9 = 72 MHz (!!!) ***/ 
    RCC->CFGR &= (uint32_t)((DWORD)~(RCC_CFGR_PLLSRC|RCC_CFGR_PLLXTPRE|RCC_CFGR_PLLMULL));
/*	PLLSRC:PLL entry clock source + PLLXTPRE:HSE divider for PLL entry + PLLMUL:PLL multiplication factor	*/
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source 	***			SW:System clock switch */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
  }
  
/* Vector Table Relocation in Internal FLASH. */
SCB->VTOR = FLASH_BASE;

/********************************************************************************************
									Настройка периферии
********************************************************************************************/
	/* включаем тактирование нужной периферии */
	//RCC->AHBENR |=(RCC_AHBENR_DMA1EN);	// DMA1
	RCC->APB2ENR =(RCC_APB2ENR_IOPAEN|
					RCC_APB2ENR_IOPBEN|
					RCC_APB2ENR_IOPCEN|
					RCC_APB2ENR_IOPDEN|
					RCC_APB2ENR_AFIOEN);	// ABCD + AF = Enable
	
/***	Бесценная информация	***
Bits 26:24SWJ_CFG[2:0]:Serial wire JTAG configuration

These bits are write-only (when read, the value is undefined). 
They are used to configure the SWJ and trace alternate function I/Os. 
The SWJ (Serial Wire JTAG) supports JTAG or SWD access to the Cortex debug port. 
The default state after reset is SWJ ON without trace. This allows JTAG or SW mode to be enabled 
by sending a specific sequence on the JTMS / JTCK pin.
000: Full SWJ (JTAG-DP + SW-DP): Reset State
001: Full SWJ (JTAG-DP + SW-DP) but without NJTRST
010: JTAG-DP Disabled and SW-DP Enabled
100: JTAG-DP Disabled and SW-DP Disabled
Other combinations: no effect
*** Отключение порта отладки (если его пины используются как обычные) ***/
//AFIO->MAPR &=(0xF0FFFFFF);
//AFIO->MAPR |=AFIO_MAPR_SWJ_CFG_DISABLE;
}
//===========================================================================================
void ConfigPorts()
{
	GPIOA->CRL		&=(xx0);	// USB_PWR_IN
	GPIOA->CRL		|=(IN0P);	// вход с подтяжкой
	PULLUP_USB_PWR_IN	=1;		// включ. подтяжки к "+"
	
	GPIOC->CRH		&=(xx9&xx12);	// кнопка и USB_PWR_OUT - управление транзистором
	GPIOC->CRH		|=(IN9|OUT12);	// вход|выход
	//USB_PWR_OUT		=1;		// выключение транзистора
	
	GPIOA->CRL		&=(xx2);	// GND точка
	GPIOA->CRL		|=(OUT2);	// 
	GPIOA->ODR		&=(~(1<<2));// даем 0 на PORTA.2 (конт. D1)

	GPIOA->CRL		&=(xx1&xx5);	// LED1, LED2
	GPIOA->CRL		|=(OUT1|OUT5);	// выход push-pull
}
//===========================================================================================

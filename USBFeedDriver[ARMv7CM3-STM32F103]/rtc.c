// rtc.c
// 23.03.2014

#include <stm32f10x.h>
#include "rtc.h"
#include "events.h"

//1. Poll RTOFF, wait until its value goes to ‘1’
//2. Set the CNF bit to enter configuration mode
//3. Write to one or more RTC registers
//4. Clear the CNF bit to exit configuration mode
//5. Poll RTOFF, wait until its value goes to ‘1’ to check the end of the write operation.

int IniRTC(void)
{
//разрешить тактирование модулей управления питанием и управлением резервной областью
RCC->APB1ENR |= RCC_APB1ENR_PWREN|RCC_APB1ENR_BKPEN;
//разрешить доступ к области резервных данных
PWR->CR |= PWR_CR_DBP;
while(!(RTC->CRL & RTC_CRL_RTOFF));	// ждем завершения операций записи RTC
RTC->CRL  =RTC_CRL_CNF;             //разрешить изменение регистров RTC

	//выполнить сброс области резервных данных
	RCC->BDCR |=RCC_BDCR_BDRST;
	RCC->BDCR &=(~RCC_BDCR_BDRST);
	//управление тактированием
	RCC->BDCR |=RCC_BDCR_RTCSEL_HSE;     //HSE-генератор / 128
	RCC->BDCR |=RCC_BDCR_RTCEN;          //подать тактирование
	 
	RTC->PRLH	=0;
	RTC->PRLL	=8000000/128;			// 62500 - настроить входной делитель 1 сек
	RTC->CNTH	=0;
	RTC->CNTL	=0;
	RTC->CRH	=RTC_CRH_SECIE;			//разрешить прерывание от секундных импульсов
	
RTC->CRL  &=(~RTC_CRL_CNF);			//запретить изменение регистров RTC
while(!(RTC->CRL & RTC_CRL_RSF));	// ждем синхронизации RTC_CNT, RTC_ALR, RTC_PRL
while(!(RTC->CRL & RTC_CRL_RTOFF));	// ждем завершения операций записи RTC

NVIC_EnableIRQ(RTC_IRQn);			//разрешение прерываний от модуля RTC
return 0;
}
//====================================================================================
void RTC_IRQHandler(void)
{
	if(RTC->CRL & RTC_CRL_SECF)	// новая секунда
	{
	RTC->CRL &=(~RTC_CRL_SECF);
		event |=NEWSEC_EVT;
	}
	else	if(RTC->CRL & RTC_CRL_ALRF)	// будильник
			{
			RTC->CRL &=(~RTC_CRL_ALRF);
				
			}
			else	if(RTC->CRL & RTC_CRL_OWF)	// перепонение счетчика
					{
					RTC->CRL &=(~RTC_CRL_OWF);
						
					}
}
//====================================================================================

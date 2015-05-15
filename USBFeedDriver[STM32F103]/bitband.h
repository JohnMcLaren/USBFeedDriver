// bitband.h
#include	<stm32f10x.h>
#include 	"types.h"
/**********************/
/***	for RAM		***/
/**********************/
#define	SETBITRAM(pReg,bit)(*(volatile DWORD*)(SRAM_BB_BASE|((pReg-SRAM_BASE)<<5)|((bit)<<2))=1)
#define	CLRBITRAM(pReg,bit)(*(volatile DWORD*)(SRAM_BB_BASE|((pReg-SRAM_BASE)<<5)|((bit)<<2))=0)
#define GETBITRAM(pReg,bit)(*(volatile DWORD*)(SRAM_BB_BASE|((pReg-SRAM_BASE)<<5)|((bit)<<2)))
#define DEFBITRAM(pReg,bit)(*(volatile DWORD*)(SRAM_BB_BASE|((pReg-SRAM_BASE)<<5)|((bit)<<2)))

/******************************************/
/***	for PORTs and other periph		***/
/******************************************/
#define	SETBITPPH(pReg,bit)(*(volatile DWORD*)(PERIPH_BB_BASE|((pReg-PERIPH_BASE)<<5)|((bit)<<2))=1)
#define	CLRBITPPH(pReg,bit)(*(volatile DWORD*)(PERIPH_BB_BASE|((pReg-PERIPH_BASE)<<5)|((bit)<<2))=0)
#define GETBITPPH(pReg,bit)(*(volatile DWORD*)(PERIPH_BB_BASE|((pReg-PERIPH_BASE)<<5)|((bit)<<2)))
#define DEFBITPPH(pReg,bit)(*(volatile DWORD*)(PERIPH_BB_BASE|((pReg-PERIPH_BASE)<<5)|((bit)<<2)))

/******************************************/
/***		Some PORTs REGs				***/
/******************************************/
#define	GPIOA_IDR	GPIOA_BASE + 0x08
#define	GPIOB_IDR	GPIOB_BASE + 0x08
#define	GPIOC_IDR	GPIOC_BASE + 0x08
#define	GPIOD_IDR	GPIOD_BASE + 0x08
#define	GPIOE_IDR	GPIOE_BASE + 0x08

#define	GPIOA_ODR	GPIOA_BASE + 0x0C
#define	GPIOB_ODR	GPIOB_BASE + 0x0C
#define	GPIOC_ODR	GPIOC_BASE + 0x0C
#define	GPIOD_ODR	GPIOD_BASE + 0x0C
#define	GPIOE_ODR	GPIOE_BASE + 0x0C

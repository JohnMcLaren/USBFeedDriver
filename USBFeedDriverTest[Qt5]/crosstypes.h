// crosstypes.h
// 28.05.2015
//------------------------------------------------------------
#ifndef CROSSTYPES_H
#define CROSSTYPES_H

#ifndef BASETYPES
//...................................................... 8-bit
typedef unsigned char			UCHAR;
typedef UCHAR		 		BYTE;
typedef BYTE				BOOL;
//...................................................... 16-bit
typedef short int			SHORT;
typedef unsigned short int		USHORT;
typedef USHORT				WORD;
typedef	USHORT				wchar;
typedef USHORT				WCHAR;
//...................................................... 32-bit
typedef long 					LONG;	/* OS dependence 32/64-bit */
typedef unsigned long 			ULONG;
typedef unsigned int			UINT;	/* always 32-bit */
typedef UINT				DWORD;
//...................................................... 64-bit
typedef long long			LONGLONG;
typedef unsigned long long 		ULONGLONG;
#endif

typedef ULONGLONG			QWORD;
//------------------------------------------------------ zero pointer
#ifndef NULL
	#define NULL				0
#endif
//...................................................... multipliers of frequencies
#define	GHz		*1000000000
#define	MHz		*1000000
#define	kHz		*1000
//---------------------------------------------------------------
#endif // CROSSTYPES_H

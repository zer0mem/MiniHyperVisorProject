#ifndef _COMMON_H
#define _COMMON_H

#include "wdm.h"
#include <stdlib.h>
#include <new>

#ifdef _WIN64
EXTERN_C void __kebreak();
#define KeBreak __kebreak
#else
__inline void KeBreak()
{
	__asm int 3;
	return;
}
#endif // _WIN64

#define MAX_PROCID (sizeof(ULONG) << 3) //*8 .. byte => 8bite
#define	PROCID(cpu)		(KAFFINITY)((ULONG_PTR)KeQueryActiveProcessors() & (ULONG_PTR)(1 << (USHORT)cpu))

// windows types
typedef unsigned long long  QWORD, *PQWORD, *LPQWORD;
typedef unsigned long	DWORD,	*PDWORD,	*LPDWORD;
typedef unsigned short	WORD,	*PWORD,		*LPWORD;
typedef unsigned char	BYTE,	*PBYTE,		*LPBYTE;
typedef unsigned int	UINT,	*PUINT,		*LPUINT;
typedef int				BOOL,	*PBOOL,		*LPBOOL;
typedef void					*PVOID,		*LPVOID;

typedef signed char SBYTE;
typedef signed short SWORD;
typedef signed long int SDWORD;

typedef char CHAR;
typedef wchar_t WCHAR;
typedef short SHORT;
typedef long LONG;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

typedef BYTE BOOLEAN;

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

typedef const void *LPCVOID;

#endif // _COMMON_H

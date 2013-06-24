// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DRIVER_TEMPLATE)
#define AFX_STDAFX_H__DRIVER_TEMPLATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN64
 #define _AMD64_
 #define _M_AMD64_
#else //WIN64
 #define i386 1
 #define _X86_ 1
 #define CONDITION_HANDLING 1
 #define _NT1X_ 100
 #define _WIN32_WINNT 0x0400 /* minimum required OS version */
#endif //WIN64

#define STD_CALL
#define NT_INST 0
#define WINNT 1
#define WIN32_LEAN_AND_MEAN 1
#define DEVL 1
#define FPO 1

extern "C" {
#pragma pack(push, 8)
#ifdef _AMD64_
  #pragma warning(push)               
  #pragma warning (disable : 4430)
#endif
  #include <ntddk.h>                  // various NT definitions
#ifdef _AMD64_
  #pragma warning(pop)               
#endif
//#include <ntifs.h>
#pragma pack(pop)
};

// TODO: Reference additional headers your program requires here
// NOTE: It worth adding here rerely changed headers
//       Put references to frequently updated headers in 'drv_common.h'


#endif // !defined(AFX_STDAFX_H__DRIVER_TEMPLATE)

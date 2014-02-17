/**
 * @file drv_common.h
 * @author created by: Peter Hlavaty
 */

#ifndef __DRV_COMMON_H__
#define __DRV_COMMON_H__

#define AFX_STDAFX_H__DRIVER_TEMPLATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NT_INST 0
#define WINNT 1
#define WIN32_LEAN_AND_MEAN 1
#define DEVL 1
#define FPO 1

extern "C" {
#ifdef _WIN64
#include <ntifs.h>
#else
#include <ntddk.h>
#endif
};

#endif //__DRV_COMMON_H__

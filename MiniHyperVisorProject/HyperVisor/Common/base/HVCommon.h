/**
 * @file HVCommon.h
 * @author created by: Peter Hlavaty
 */

#ifndef __HVCOMMON_H__
#define __HVCOMMON_H__

#include "new.h"
#include "ntddk.h"

#include "../../../Common/CPU/msr.h"
#include "../../Common/CPU/vmcs.h"

#include "../../../Common/base/instrinsics.h"


#pragma pack(push, 1)

typedef struct _SEGMENT_DECRIPTOR
{
	unsigned        LimitLow                :16;
	unsigned        BaseLow                 :16;
	unsigned        BaseMid                 :8;
	unsigned        AttributesLow			:8;
	unsigned        LimitHigh				:4;
	unsigned        AttributesHigh			:4;
	unsigned        BaseHigh                :8;
}SEGMENT_DESCRIPTOR;

struct SEGMENT_SELECTOR
{
	ULONG_PTR	selector;
	DWORD		limit;
	ULONG_PTR   base;
	ULONG_PTR   rights;
	USHORT		attributes;
};

typedef struct _GDT
{
	WORD		limit;
	ULONG_PTR	base;
}GDT;

typedef struct _VMCS
{
	PVOID				pvmcs;
	PHYSICAL_ADDRESS	vmcs;
}VMCS;

typedef struct _GUEST_STATE
{
	KAFFINITY	ProcId;
	VMCS		GVmcs;
	VMCS		HVmcs;
	PBYTE		PIN;
	PBYTE		PROC;
	PBYTE		EXIT;
	PBYTE		ENTRY;
	PBYTE		SEIP;
	PBYTE		SESP;
	GDT			Gdtr;
	GDT			Idtr;
	ULONG_PTR	Ldtr;
	ULONG_PTR	Tr;
	ULONG_PTR	Gs;
	ULONG_PTR	Fs;
	ULONG_PTR	Ss;
	ULONG_PTR	Es;
	ULONG_PTR	Ds;
	ULONG_PTR	Cs;
	ULONG_PTR	CR4;
	ULONG_PTR	CR3;
	ULONG_PTR	CR0;
	ULONG_PTR* HRSP;
	ULONG_PTR	RFLAGS;
	const VOID*	HRIP;
}GUEST_STATE;

#pragma pack(pop)


#define MIN_PAGE_SIZE			0x1000
#define VMCS_PAGE_SIZE			(MIN_PAGE_SIZE)
#define HYPERVISOR_STACK_PAGE	(2 * MIN_PAGE_SIZE)

#define CALL(proc)				((VOID (*)()) & proc)

#define SEG_DATA				0x10
#define SEG_CODE				0x18
#define SEG_Q_LIMIT				0xFFFFFFFFFFFFFFFF
#define SEG_D_LIMIT				SEG_Q_LIMIT

#define NORMAL					0x10
#define	IS_GRANULARITY_4KB		0xB

#define BTS(b)					(1 << b)
#define CR4_VMXE				(ULONG)BTS(13)
#define CR0_PG					(ULONG)BTS(31)
#define CR0_NE					(ULONG)BTS(5)
#define CR0_PE					(ULONG)BTS(0)

#define TAG						0x5153454E
#define	MAX_CALLBACK			VMX_EXIT_XSETBV+2
#define MAGIC					0x4C495052


#define	RAX			0
#define	RCX			1
#define	RDX			2
#define	RBX			3
#define	RBP			5
#define	RSI			6
#define	RDI			7


#define G_GS		0
#define G_FS		G_GS+1
#define G_ES		G_GS+2
#define G_DS		G_GS+3

#define G_RIP		G_DS+1
#define G_CS 		G_RIP+1
#define G_RFLAGS	G_RIP+2
#define G_RSP		G_RIP+3
#define G_SS		G_RIP+4

#define MAX_PROCID (sizeof(ULONG) << 3) //*8 .. byte => 8bite

#define	PROCID(cpu)		(KAFFINITY)((ULONG_PTR)KeQueryActiveProcessors() & (ULONG_PTR)(1 << (USHORT)cpu))

#define ALIGN(addr, granularity)	(ULONG_PTR)((ULONG_PTR)addr & (~(granularity - 1)))

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                  \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

#define kCpuidMark	MAKEFOURCC('P', 'I', 'L', 'L')
#define kStackMark	MAKEFOURCC('C', 'O', 'L', 'D')

EXTERN_C ULONG_PTR __x64_cpuid(ULONG_PTR);

#endif //__HVCOMMON_H__

/**
 * @file HVCommon.h
 * @author created by: Peter Hlavaty
 */

#ifndef __HVCOMMON_H__
#define __HVCOMMON_H__

#include "new.h"
#include "wdm.h"

#include "../../../Common/CPU/msr.h"
#include "../../Common/CPU/vmcs.h"

#include "../../../Common/base/instrinsics.h"


#pragma pack(push, 1)

struct GATE_DESCRIPTOR
{
	WORD Offset;
	DWORD Access;
	WORD Selector;
	DWORD ExtendedOffset;
	DWORD Reserved;
};

struct SEGMENT_DESCRIPTOR
{
	ULONG_PTR LimitLow : 16;
	ULONG_PTR BaseLow : 16;
	ULONG_PTR BaseMid : 8;
	ULONG_PTR AttributesLow : 8;
	ULONG_PTR LimitHigh : 4;
	ULONG_PTR AttributesHigh : 4;
	ULONG_PTR BaseHigh : 8;
};

//SEGMENT_DESCRIPTOR.Attributes (-Gap)...
typedef union
{
	USHORT UCHARs;
	struct FIELDS
	{
		USHORT type:4;              /* 0;  Bit 40-43 */
		USHORT s:1;                 	/* 4;  Bit 44 */
		USHORT dpl:2;               	/* 5;  Bit 45-46 */
		USHORT p:1;                 	/* 7;  Bit 47 */
		// gap!   (this will be explained later)     
		USHORT avl:1;               	/* 8;  Bit 52 */
		USHORT l:1;                 	/* 9;  Bit 53 */
		USHORT db:1;                	/* 10; Bit 54 */
		USHORT g:1;                 	/* 11; Bit 55 */
		USHORT Gap:4;
	};
} SEGMENT_ATTRIBUTES;

struct SEGMENT_SELECTOR
{
	ULONG_PTR selector;
	DWORD limit;
	ULONG_PTR base;
	ULONG_PTR rights;
	USHORT attributes;
};

struct GDT
{
	WORD limit;
	ULONG_PTR base;
};

struct VMCS
{
	PVOID pvmcs;
	PHYSICAL_ADDRESS vmcs;
};

struct GUEST_STATE
{
	KAFFINITY ProcId;
	VMCS GVmcs;
	VMCS HVmcs;
	PBYTE PIN;
	PBYTE PROC;
	PBYTE EXIT;
	PBYTE ENTRY;
	PBYTE SEIP;
	PBYTE SESP;
	GDT Gdtr;
	GDT Idtr;
	ULONG_PTR Ldtr;
	ULONG_PTR Tr;
	ULONG_PTR Gs;
	ULONG_PTR Fs;
	ULONG_PTR Ss;
	ULONG_PTR Es;
	ULONG_PTR Ds;
	ULONG_PTR Cs;
	ULONG_PTR CR4;
	ULONG_PTR CR3;
	ULONG_PTR CR0;
	ULONG_PTR* HRSP;
	ULONG_PTR RFLAGS;
	const VOID*	HRIP;
};

#pragma pack(pop)

#define LBR 1
#define BTF 2
#define TRAP 0x100

#define HYPERVISOR_STACK_PAGE	(2 * PAGE_SIZE)

#define CALL(proc)				((VOID (*)()) & proc)

#define SEG_DATA				0x10
#define SEG_CODE				0x18
#define SEG_Q_LIMIT				0xFFFFFFFFFFFFFFFF
#define SEG_D_LIMIT				SEG_Q_LIMIT

#define NORMAL					0x10
#define	IS_GRANULARITY_4KB		0xB

#define BTS(b)					(1 << b)
#define CR4_VMXE				static_cast<ULONG>(BTS(13))
#define CR4_DE					static_cast<ULONG>(BTS(3))
#define CR0_PG					static_cast<ULONG>(BTS(31))
#define CR0_NE					static_cast<ULONG>(BTS(5))
#define CR0_PE					static_cast<ULONG>(BTS(0))

#define FEATURE_CONTROL_LOCKED			BTS(0)
#define FEATURE_CONTROL_VMXON_ENABLED	BTS(2)

#define	MAX_CALLBACK			VMX_EXIT_XSETBV+2

enum
{
	G_GS = 0,
	G_FS,
	G_ES,
	G_DS,
	G_RIP,
	G_CS,
	G_RFLAGS,
	G_RSP,
	G_SS
};

#define MAX_PROCID (sizeof(ULONG) << 3) //*8 .. byte => 8bite

#define	PROCID(cpu) (KAFFINITY)((ULONG_PTR)KeQueryActiveProcessors() & (ULONG_PTR)(1 << (USHORT)cpu))

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                  \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

#define kCpuidMark	MAKEFOURCC('P', 'I', 'L', 'L')
#define kStackMark	MAKEFOURCC('C', 'O', 'L', 'D')

EXTERN_C ULONG_PTR __x64_cpuid(ULONG_PTR);

#endif //__HVCOMMON_H__

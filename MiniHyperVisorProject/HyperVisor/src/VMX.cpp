/**
 * @file VMX.cpp
 * @author created by: Peter Hlavaty
 */

#include "StdAfx.h"

#include "VMX.h"
#include "../../Common/Kernel/IRQL.hpp"

CVmx::CVmx(KAFFINITY procId) : m_cpuActivated(false)
{
	RtlZeroMemory(&m_guestState, sizeof(m_guestState));
	m_preparedState = GetGuestState(procId);
}

__checkReturn bool CVmx::InstallHyperVisor( __in const VOID* hvEntryPoint, __in VOID* hvStack )
{
	if (m_preparedState)
	{
		m_guestState.HRIP = hvEntryPoint;
		m_guestState.HRSP = (ULONG_PTR*)ALIGN(hvStack, 0x100);

		m_cpuActivated = VmcsInit();
	}

	return m_cpuActivated;
}

CVmx::~CVmx()
{
	if (m_preparedState)
	{
		MmFreeNonCachedMemory(m_guestState.GVmcs.pvmcs, VMCS_PAGE_SIZE);	
		MmFreeNonCachedMemory(m_guestState.HVmcs.pvmcs, VMCS_PAGE_SIZE);
	}
}

__checkReturn bool CVmx::IsVirtualizationEnabled()
{
	return (0 != (rdmsr(IA32_FEATURE_CONTROL_CODE) & FEATURE_CONTROL_VMXON_ENABLED));
}

__checkReturn bool CVmx::IsVirtualizationLocked()
{
	return (0 != (rdmsr(IA32_FEATURE_CONTROL_CODE) & FEATURE_CONTROL_LOCKED));
}

void CVmx::EnableVirtualization()
{
	DbgPrint("Virtualization is trying to enable via wrmsr setting FEATURE_CONTROL_VMXON_ENABLED");
	wrmsr(IA32_FEATURE_CONTROL_CODE, rdmsr(IA32_FEATURE_CONTROL_CODE) | FEATURE_CONTROL_VMXON_ENABLED);
}

__checkReturn bool CVmx::VmcsInit()
{
	ULONG_PTR guest_rsp;
	ULONG_PTR guest_rip;
	get_guest_exit(&guest_rsp, &guest_rip);

	if (m_cpuActivated)
		return true;

	KeSetSystemAffinityThread(m_guestState.ProcId);

	if (!IsVirtualizationEnabled())
	{
		if (IsVirtualizationLocked())
		{
			DbgPrint("\nVirtualization not supported from BIOS! [%x]", rdmsr(IA32_FEATURE_CONTROL_CODE));
			return false;
		}

		EnableVirtualization();
	}
	DbgPrint("\nVirtualization is enabled!");

	{
		CDisableInterrupts idis;
		writecr0(m_guestState.CR0);
		writecr4(m_guestState.CR4);
	}

	*((ULONG_PTR*)(m_guestState.GVmcs.pvmcs)) = rdmsr(IA32_VMX_BASIC_MSR_CODE);
	*((ULONG_PTR*)(m_guestState.HVmcs.pvmcs)) = rdmsr(IA32_VMX_BASIC_MSR_CODE);

	vmxon(&(m_guestState.HVmcs.vmcs));

	vmclear(&(m_guestState.GVmcs.vmcs));

	vmptrld(&(m_guestState.GVmcs.vmcs));

	//GLOBALS
	vmwrite(VMX_VMCS_CTRL_ENTRY_MSR_LOAD_COUNT, 0);
	vmwrite(VMX_VMCS_CTRL_EXIT_MSR_LOAD_COUNT, 0);
	vmwrite(VMX_VMCS_CTRL_EXIT_MSR_STORE_COUNT, 0);

	SetCRx();
	SetControls();

	SetDT();
	SetSysCall();

	//GUEST
	vmwrite(VMX_VMCS_GUEST_LINK_PTR_FULL, -1);
	vmwrite(VMX_VMCS_GUEST_LINK_PTR_HIGH, -1);

	vmwrite(VMX_VMCS_GUEST_DEBUGCTL_FULL, rdmsr(IA32_DEBUGCTL) & SEG_D_LIMIT);
	vmwrite(VMX_VMCS_GUEST_DEBUGCTL_HIGH, rdmsr(IA32_DEBUGCTL) >> 32);

	//SELECTORS
	SetSegSelector(m_guestState.Cs, VMX_VMCS16_GUEST_FIELD_CS);

	SetSegSelector(m_guestState.Ds, VMX_VMCS16_GUEST_FIELD_DS);
	SetSegSelector(m_guestState.Es, VMX_VMCS16_GUEST_FIELD_ES);
	SetSegSelector(m_guestState.Ss, VMX_VMCS16_GUEST_FIELD_SS);	
	SetSegSelector(m_guestState.Fs, VMX_VMCS16_GUEST_FIELD_FS);
	SetSegSelector(m_guestState.Gs, VMX_VMCS16_GUEST_FIELD_GS);	

	SetSegSelector(m_guestState.Ldtr, VMX_VMCS16_GUEST_FIELD_LDTR);
	SetSegSelector(m_guestState.Tr, VMX_VMCS16_GUEST_FIELD_TR);

	vmwrite(VMX_VMCS64_GUEST_FS_BASE, rdmsr(IA32_FS_BASE));
	vmwrite(VMX_VMCS64_GUEST_GS_BASE, rdmsr(IA32_GS_BASE));

	//HOST
	vmwrite(VMX_VMCS16_HOST_FIELD_CS, m_guestState.Cs);
	vmwrite(VMX_VMCS16_HOST_FIELD_DS, SEG_DATA);
	vmwrite(VMX_VMCS16_HOST_FIELD_ES, SEG_DATA);
	vmwrite(VMX_VMCS16_HOST_FIELD_SS, m_guestState.Ss);
	vmwrite(VMX_VMCS16_HOST_FIELD_FS, m_guestState.Fs & 0xf8);
	vmwrite(VMX_VMCS16_HOST_FIELD_GS, m_guestState.Gs & 0xf8);
	vmwrite(VMX_VMCS16_HOST_FIELD_TR, m_guestState.Tr);


	RtlZeroMemory((PVOID)((ULONG_PTR)m_guestState.GVmcs.pvmcs + 4), 4);


	vmwrite(VMX_VMCS64_GUEST_RSP, guest_rsp);
	vmwrite(VMX_VMCS64_GUEST_RIP, guest_rip);
	vmwrite(VMX_VMCS_GUEST_RFLAGS, m_guestState.RFLAGS);

	vmwrite(VMX_VMCS_HOST_RSP, m_guestState.HRSP);
	vmwrite(VMX_VMCS_HOST_RIP, m_guestState.HRIP);

	m_cpuActivated = true;

	vmlaunch();

	KeBreak();
	DbgPrint("\nhv on failed\n");
	return false;
}

__checkReturn bool CVmx::GetGuestState( __in KAFFINITY procId )
{
	m_guestState.ProcId = procId;
	KeSetSystemAffinityThread(procId);

	m_guestState.CR0 = (readcr0() & rdmsr(IA32_VMX_CR0_FIXED1)) | rdmsr(IA32_VMX_CR0_FIXED0) | CR0_PE | CR0_NE | CR0_PG;
	m_guestState.CR4 = (readcr4() & rdmsr(IA32_VMX_CR4_FIXED1)) | rdmsr(IA32_VMX_CR4_FIXED0) | CR4_VMXE;

	m_guestState.CR3 = readcr3();
	m_guestState.RFLAGS = readeflags();

	m_guestState.Cs = readcs();
	m_guestState.Ds = readds();
	m_guestState.Es = reades();
	m_guestState.Ss = readss();
	m_guestState.Fs = readfs();
	m_guestState.Gs = readgs();

	m_guestState.PIN = (PBYTE)(rdmsr(IA32_VMX_PINBASED_CTLS) & SEG_D_LIMIT);
	m_guestState.PROC = (PBYTE)(rdmsr(IA32_VMX_PROCBASED_CTLS) & SEG_D_LIMIT);
	m_guestState.EXIT = (PBYTE)((rdmsr(IA32_VMX_EXIT_CTLS) & SEG_D_LIMIT | (1 << 15)) | VMX_VMCS32_EXIT_IA32E_MODE | VMX_VMCS32_EXIT_ACK_ITR_ON_EXIT);
	m_guestState.ENTRY = (PBYTE)((rdmsr(IA32_VMX_ENTRY_CTLS)& SEG_D_LIMIT) | VMX_VMCS32_ENTRY_IA32E_MODE);
	m_guestState.SEIP = (PBYTE)(rdmsr(IA64_SYSENTER_EIP) & SEG_D_LIMIT);
	m_guestState.SESP = (PBYTE)(rdmsr(IA32_SYSENTER_ESP) & SEG_D_LIMIT);

	m_guestState.GVmcs.pvmcs = MmAllocateNonCachedMemory(VMCS_PAGE_SIZE);
	if (NULL == m_guestState.GVmcs.pvmcs)
		return false;

	RtlZeroMemory(m_guestState.GVmcs.pvmcs, VMCS_PAGE_SIZE);
	m_guestState.GVmcs.vmcs = MmGetPhysicalAddress(m_guestState.GVmcs.pvmcs);

	m_guestState.HVmcs.pvmcs = MmAllocateNonCachedMemory(VMCS_PAGE_SIZE);
	if (NULL == m_guestState.HVmcs.pvmcs)
		return false;

	RtlZeroMemory(m_guestState.HVmcs.pvmcs, VMCS_PAGE_SIZE);
	m_guestState.HVmcs.vmcs = MmGetPhysicalAddress(m_guestState.HVmcs.pvmcs);


	sgdt(&(m_guestState.Gdtr));
	sidt(&(m_guestState.Idtr));

	m_guestState.Tr = str();
	m_guestState.Ldtr = sldt();

	return true;
}

void CVmx::SetCRx( )
{
	vmwrite(VMX_VMCS_CTRL_CR0_READ_SHADOW, CR0_PG);//PG
	vmwrite(VMX_VMCS_CTRL_CR4_READ_SHADOW, 0);
	vmwrite(VMX_VMCS_CTRL_CR3_TARGET_COUNT, 0);

	//CR GUEST
	vmwrite(VMX_VMCS64_GUEST_CR0, m_guestState.CR0);	
	vmwrite(VMX_VMCS64_GUEST_CR3, m_guestState.CR3);
	vmwrite(VMX_VMCS64_GUEST_CR4, m_guestState.CR4);
	vmwrite(VMX_VMCS64_GUEST_DR7, 0x400);

	//CR HOST
	vmwrite(VMX_VMCS_HOST_CR0, m_guestState.CR0);
	vmwrite(VMX_VMCS_HOST_CR3, m_guestState.CR3);
	vmwrite(VMX_VMCS_HOST_CR4, m_guestState.CR4);
}

void CVmx::SetControls( )
{
	vmwrite(VMX_VMCS_CTRL_PIN_EXEC_CONTROLS, m_guestState.PIN);
	vmwrite(VMX_VMCS_CTRL_PROC_EXEC_CONTROLS, m_guestState.PROC);
	vmwrite(VMX_VMCS_CTRL_EXIT_CONTROLS, m_guestState.EXIT);
	vmwrite(VMX_VMCS_CTRL_ENTRY_CONTROLS, m_guestState.ENTRY);
}

void CVmx::SetDT( )
{
	vmwrite(VMX_VMCS64_GUEST_IDTR_BASE,m_guestState.Idtr.base);
	vmwrite(VMX_VMCS32_GUEST_IDTR_LIMIT, m_guestState.Idtr.limit);
	vmwrite(VMX_VMCS64_GUEST_GDTR_BASE, m_guestState.Gdtr.base);
	vmwrite(VMX_VMCS32_GUEST_GDTR_LIMIT, m_guestState.Gdtr.limit);	

	vmwrite(VMX_VMCS_HOST_FS_BASE, rdmsr(IA32_FS_BASE)&SEG_Q_LIMIT);
	SEGMENT_SELECTOR seg_sel;
	GetSegmentDescriptor(&seg_sel, m_guestState.Tr);
	vmwrite(VMX_VMCS_HOST_TR_BASE, seg_sel.base);
	vmwrite(VMX_VMCS_HOST_GDTR_BASE, m_guestState.Gdtr.base);
	vmwrite(VMX_VMCS_HOST_IDTR_BASE, m_guestState.Idtr.base);
}

void CVmx::SetSysCall( )
{
	vmwrite(VMX_VMCS32_GUEST_SYSENTER_CS,rdmsr(IA32_SYSENTER_CS) & SEG_D_LIMIT);
	vmwrite(VMX_VMCS64_GUEST_SYSENTER_ESP, m_guestState.SESP);
	vmwrite(VMX_VMCS64_GUEST_SYSENTER_EIP, m_guestState.SEIP);


	vmwrite(VMX_VMCS32_HOST_SYSENTER_CS, rdmsr(IA32_SYSENTER_CS) & SEG_D_LIMIT);
	vmwrite(VMX_VMCS_HOST_SYSENTER_EIP, m_guestState.SEIP);
	vmwrite(VMX_VMCS_HOST_SYSENTER_ESP, m_guestState.SESP);
}

void CVmx::GetSegmentDescriptor( __out SEGMENT_SELECTOR* segSel, __in ULONG_PTR selector )
{
	RtlZeroMemory(segSel, sizeof(SEGMENT_SELECTOR));	
	SEGMENT_DESCRIPTOR* seg = (SEGMENT_DESCRIPTOR *)((PUCHAR) m_guestState.Gdtr.base + (selector >> 3) * 8);	

	segSel->selector = selector;
	segSel->limit =	seg->LimitLow | (seg->LimitHigh << 16);
	segSel->base = seg->BaseLow | (seg->BaseMid << 16) | (seg->BaseHigh << 24);
	segSel->attributes = seg->AttributesLow | (seg->AttributesHigh << 8);

	//is TSS or CALLBACK ?
	if (!(seg->AttributesLow & NORMAL))
		segSel->base = (segSel->base & SEG_D_LIMIT) | ((*(PULONG64) ((PUCHAR)seg + 8)) << 32);

	if (segSel->attributes >> IS_GRANULARITY_4KB == 1)
		segSel->limit = (segSel->limit << 12) | 0xFFFF;

	segSel->rights = (segSel->selector ? (((PUCHAR) &segSel->attributes)[0] + (((PUCHAR) &segSel->attributes)[1] << 12)) : 0x10000);
}

void CVmx::SetSegSelector( __in ULONG_PTR segSelector, __in ULONG_PTR segField )
{
	ULONG_PTR index = (segField - VMX_VMCS16_GUEST_FIELD_ES) / sizeof(WORD) * 2;

	SEGMENT_SELECTOR seg_sel;
	GetSegmentDescriptor(&seg_sel, segSelector);

	vmwrite(VMX_VMCS32_GUEST_ES_LIMIT + index, seg_sel.limit);	
	vmwrite(VMX_VMCS32_GUEST_ES_ACCESS_RIGHTS + index, seg_sel.rights);
	vmwrite(VMX_VMCS16_GUEST_FIELD_ES + index, segSelector);
	vmwrite(VMX_VMCS64_GUEST_ES_BASE + index, seg_sel.base);
}

#include "CSysCall.h"
#include <HyperVisor/Common/base/instrinsics.h>
#include <Common/cpu/msr.h>

void* CSysCall::m_syscalls[MAX_PROCID];

extern "C"
void 
sysenter();

extern "C"
void 
rdmsr_hook();

CSysCall::CSysCall() : 
	CCRonos()
{
	RtlZeroMemory(m_syscalls, sizeof(m_syscalls));
}

CSysCall::~CSysCall()
{
	BYTE core_id = 0;
	CProcessorWalker cpu_w;
	while (cpu_w.NextCore(&core_id))
	{
		KeSetSystemAffinityThread(PROCID(core_id));

		HookSyscallMSR(m_syscalls[core_id]);

		DbgPrint("Unhooked. procid [%x] <=> syscall addr [%p]\n", core_id, m_syscalls[core_id]);
		core_id++;
	}
}

void 
CSysCall::Install()
{
	if (CCRonos::EnableVirtualization())
	{
		for (BYTE i = 0; i < m_vCpu.GetCount(); i++)
		{

#if HYPERVISOR

			if (m_vCpu[i].VirtualizationON())

#endif

			{
				int CPUInfo[4] = {0};
				int InfoType = 0;
				__cpuid(CPUInfo, InfoType);
				DbgPrint("\r\n~~~~~~~~~~~ CPUID (%i) : %s ~~~~~~~~~~~\r\n", i, CPUInfo);

				HookSyscallMSR(sysenter);
				DbgPrint("II. procid [%x] <=> syscall addr [%p]\n\n", i, (ULONG_PTR)rdmsr(IA64_SYSENTER_EIP));		
			}
		}
	}

}

__checkReturn
bool 
CSysCall::SetVirtualizationCallbacks()
{
	DbgPrint("CSysCall::SetVirtualizationCallbacks\n");

	if (!CCRonos::SetVirtualizationCallbacks())
		return false;

	m_traps[VMX_EXIT_RDMSR] = HookProtectionMSR;

	return true;//RegisterCallback(m_callbacks, );
}


void 
CSysCall::PerCoreAction( 
	__in BYTE coreId 
	)
{
	CCRonos::PerCoreAction(coreId);

	if (coreId < sizeof(m_syscalls))
	{
		KeSetSystemAffinityThread(PROCID(coreId));
		m_syscalls[coreId] = (void*)rdmsr(IA64_SYSENTER_EIP);
		HookSyscallMSR(sysenter);
		DbgPrint("Hooked. procid [%x] <=> syscall addr [%p]\n", coreId, m_syscalls[coreId]);
	}
}

//static 

void* 
CSysCall::GetSysCall( 
	__in BYTE coreId 
	)
{
	if (coreId > MAX_PROCID)
		return NULL;

	return CSysCall::m_syscalls[coreId];
}

void
CSysCall::HookSyscallMSR(
	__in const void* hook
	)
{
	cli();
	wrmsr(IA64_SYSENTER_EIP, (ULONG_PTR)hook);
	sti();
}

//hook

LONG64 m_counter = 0;

extern "C" 
void* 
SysCallCallback( 
	__inout ULONG_PTR* reg 
	)
{
	InterlockedIncrement64(&m_counter);
	if (0 == (m_counter % (0x100000 / 4)))
		DbgPrint("syscalls are really painfull ... : %x\n", m_counter);

	ULONG core_id = KeGetCurrentProcessorNumber();
	if (core_id > MAX_PROCID)
		core_id = 0;//incorrect ... TODO ...

	return CSysCall::GetSysCall((BYTE)core_id);
}

//****
//HV callback -> hook protection!

void 
CSysCall::HookProtectionMSR( 
	__inout ULONG_PTR reg[0x10] 
	)
{
	ULONG_PTR syscall;
	if (IA64_SYSENTER_EIP == reg[RCX])
	{
		syscall = (ULONG_PTR)CSysCall::GetSysCall(CVirtualizedCpu::GetCoreId(reg));

		EVmErrors status;
		ULONG_PTR ins_len = Instrinsics::VmRead(VMX_VMCS32_RO_EXIT_INSTR_LENGTH, &status);
		if (VM_OK(status))
		{
			ULONG_PTR eip = Instrinsics::VmRead(VMX_VMCS64_GUEST_RIP, &status);
			if (VM_OK(status))
			{
				status = Instrinsics::VmWrite(VMX_VMCS64_GUEST_RIP, reinterpret_cast<ULONG_PTR>(rdmsr_hook));//rdmsr_hook is trampolie to RdmsrHook
				if (VM_OK(status))
				{
					reg[RCX] = eip;
					m_sRdmsrRips.Push(reg[RCX] - ins_len);
				}
			}
		}
	}
	else
	{
		syscall = rdmsr(static_cast<ULONG>(reg[RCX]));
	}

	reg[RAX] = static_cast<ULONG>(syscall);
	reg[RDX] = static_cast<ULONG>(syscall >> (sizeof(ULONG) << 3));
}

//crapppy container
CStack
CSysCall::m_sRdmsrRips;

CStack& 
CSysCall::GetRdmsrStack()
{
	return m_sRdmsrRips;
}

//little bit another kind of hook -virtualization-based- :P
extern "C" 
void* 
RdmsrHook( 
	__inout ULONG_PTR* reg 
	)
{
	void* ret = (void*)reg[RCX];
	DbgPrint("\nRdmsrHook %p [pethread : %p]\n", ret, PsGetCurrentThread());
	reg[RCX] = IA64_SYSENTER_EIP;
	DbgBreakPoint();
	return ret;
}

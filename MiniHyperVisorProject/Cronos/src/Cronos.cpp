/**
 * @file Cronos.cpp
 * @author created by: Peter Hlavaty
 */

#include "drv_common.h"
#include "Cronos.h"
#include "../../Common/base/instrinsics.h"

//examples of callbacks functionality 

void HVCallback1(
	__inout ULONG_PTR reg[REG_COUNT]
	);
void HVCallback2(
	__inout ULONG_PTR reg[REG_COUNT]
	);
void HVCallback3(
	__inout ULONG_PTR reg[REG_COUNT]
	);
void HVCallback4(
	__inout ULONG_PTR reg[REG_COUNT]
	);

//ctor dtor

CCRonos::CCRonos() :
	m_vCpu(static_cast<BYTE>(KeQueryActiveProcessorCount(&m_cpu)))
{
	DbgPrint("\n>CCRonos ctor");	
	RtlZeroMemory(m_traps,sizeof(m_traps));
	RtlZeroMemory(&m_callbacks, sizeof(m_callbacks));
}

CCRonos::~CCRonos()
{
	DbgPrint("\n<CCRonos dtor");
	KeBreak();

	for (BYTE i = 0; i < m_vCpu.GetCount(); i++)
		m_vCpu[i].~CVirtualizedCpu();


	CALLBACK* callbacks = &m_callbacks;
	while (NULL != callbacks->Next)
	{
		CALLBACK* callback = callbacks;
		callbacks = callbacks->Next;
		delete callbacks;
	}
}

//virtuals

__checkReturn 
bool CCRonos::SetVirtualizationCallbacks()
{
	DbgPrint("CCRonos::SetVirtualizationCallbacks\n");
	m_traps[VMX_EXIT_CPUID] = HVCpuid;

	return (RegisterCallback(&m_callbacks, HVCallback1) &&
			RegisterCallback(&m_callbacks, HVCallback2) &&
			RegisterCallback(&m_callbacks, HVCallback3));
}

void CCRonos::PerCoreAction(
	__in BYTE coreId
	)
{
	DbgPrint("CCRonos::PerCoreAction - ");
	DbgPrint("\nLets Go start virtualizee cpu : %x !\n", coreId);
	
	::new(&(m_vCpu[coreId])) 
		CVirtualizedCpu(
			coreId, 
			m_traps, 
			!!m_traps[VMX_EXIT_EXCEPTION], 
			reinterpret_cast<VMCallback>(HVCallback), 
			&m_callbacks
			);
}

//public hv on/off

bool CCRonos::EnableVirtualization()
{
	if (m_vCpu.IsAllocated())
	{
		if (SetVirtualizationCallbacks())
		{
			BYTE coreID = 0;
			CProcessorWalker cpu_w;
			while (cpu_w.NextCore(&coreID, coreID))
			{
				PerCoreAction(coreID);
				coreID++;//follow with next core
			}

			return true;
		}
	}
	return false;
}

void CCRonos::Install()
{
	if (EnableVirtualization())
	{
		for (size_t i = 0; i < m_vCpu.GetCount(); i++)
		{
			if (m_vCpu[i].VirtualizationON())
			{
				int CPUInfo[4] = {0};
				int InfoType = 0;
				__cpuid(CPUInfo, InfoType);
				DbgPrint("\r\n~~~~~~~~~~~ CPUID (%i) : %s ~~~~~~~~~~~\r\n", i, CPUInfo);
			}
		}
	}
}

void CCRonos::StopVirtualization()
{
	if (m_vCpu.IsAllocated())
	{
		for (size_t i = 0; i < m_vCpu.GetCount(); i++)
		{
			while(!m_vCpu[i].VirtualizationOFF())
			{
				DbgPrint("\n!fail to off core : %x", i);
				KeBreak();
			}

			DbgPrint("\n<core : %x is offline", i);
		}
	}
}

//callbacks related method -> aka how to extend mechanism for generic usage ... ;)

bool CCRonos::RegisterCallback( 
	__in CALLBACK* callbacks, 
	__in const VMTrap callback 
	)
{
	while (NULL != callbacks->Next)
		callbacks = callbacks->Next;
	
	CALLBACK* t_callback = new CALLBACK;
	if (!t_callback)
		return false;

	::new(callbacks) CALLBACK(callback, t_callback);

	return true;
}


//calbacks called from HYPERVISOR!!!

//__in_opt CALLBACK** callbacks == ptr to m_callbacks
void CCRonos::HVCallback(
	__inout ULONG_PTR reg[REG_COUNT], 
	__in_opt const CALLBACK** callbacks
	)
{
	if (!callbacks)
		return;

	const CALLBACK* t_callbacks = *callbacks;
	if (!t_callbacks)
		return;

	while (t_callbacks->Callback != NULL)
	{
		t_callbacks->Callback(reg);
		t_callbacks = t_callbacks->Next;
	}
}

void CCRonos::HVCpuid( 
	__inout ULONG_PTR reg[REG_COUNT] 
	)
{
	reg[RAX] = kCpuidMark;
}


//extended hypevisor callbacks mechanism

void HVCallback1( 
	__inout ULONG_PTR reg[REG_COUNT] 
	)
{
	VM_STATUS status;
	ULONG_PTR ExitReason = Instrinsics::VmRead(VMX_VMCS32_RO_EXIT_REASON, &status);

	if (VM_OK(status))
	{
		if (VMX_EXIT_CPUID == ExitReason)
			reg[RDX] = kCpuidMark3;
	}
}

void HVCallback2( 
	__inout ULONG_PTR reg[REG_COUNT] 
	)
{
	VM_STATUS status;
	ULONG_PTR ExitReason = Instrinsics::VmRead(VMX_VMCS32_RO_EXIT_REASON, &status);

	if (VM_OK(status))
	{
		if (VMX_EXIT_CPUID == ExitReason)
			reg[RCX] = kCpuidMark2;
	}
}

void HVCallback3( 
	__inout ULONG_PTR reg[REG_COUNT] 
	)
{
	VM_STATUS status;
	ULONG_PTR ExitReason = Instrinsics::VmRead(VMX_VMCS32_RO_EXIT_REASON, &status);

	if (VM_OK(status))
	{
		if (VMX_EXIT_CPUID == ExitReason)
			reg[RBX] = kCpuidMark1;
	}
}

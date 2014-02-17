/**
 * @file HyperVisor.cpp
 * @author created by: Peter Hlavaty
 */

#include "drv_common.h"
#include "HyperVisor.h"
#include "../Common/base/instrinsics.h"

//////////////////////////////////////////////////////////////////////
// ASSEMBLY DEPENDENCIES
//////////////////////////////////////////////////////////////////////

EXTERN_C void  hv_exit();

EXTERN_C void __hv_invd();
EXTERN_C void __hv_rdmsr();
EXTERN_C void __hv_wrmsr();
EXTERN_C void __hv_cpuid();
EXTERN_C void __hv_crx();
EXTERN_C void __hv_dummy();
EXTERN_C void __hv_dummy();
EXTERN_C void __hv_vmcall();
EXTERN_C void __hv_rdtsc();

//////////////////////////////////////////////////////////////////////
// PUBLIC INTERFACE - INSTALL
//////////////////////////////////////////////////////////////////////

CHyperVisor::CHyperVisor(
	__in BYTE coreId, 
	__in_opt const VMTrap traps[MAX_CALLBACK], 
	__in_opt const VMCallback callback
	) : m_coreId()
{
	m_callback = callback ? callback : reinterpret_cast<VMCallback>(DUMMY);

	for(int i = 0; i < MAX_CALLBACK; i++)
	{
		if(NULL == traps || NULL == traps[i])
		{
			if ((i > VMX_EXIT_VMCALL) && (i <= VMX_EXIT_VMXON))
			{
				m_hvCallbacks[i] = VMX;
			}
			else
			{
				switch(i)
				{
				case VMX_EXIT_VMCALL:
					m_hvCallbacks[i] = VMCALL;
					break;
				case VMX_EXIT_RDMSR:
					m_hvCallbacks[i] = RDMSR;
					break;
				case VMX_EXIT_WRMSR:
					m_hvCallbacks[i] = WRMSR;
					break;			
				case VMX_EXIT_INVD:
					m_hvCallbacks[i] = INVD;
					break;
				case VMX_EXIT_CPUID:
					m_hvCallbacks[i] = CPUID;
					break;
				case VMX_EXIT_RDTSC:
					m_hvCallbacks[i] = RDTSC;
					break;
				default:
					m_hvCallbacks[i] = DUMMY;
				}
			}
		}		
		else
		{
			m_hvCallbacks[i] = traps[i];
		}
	}
}

CHyperVisor::~CHyperVisor()
{
}

BYTE CHyperVisor::GetCoredId()
{
	return m_coreId;
}

//////////////////////////////////////////////////////////////////////
// HYPERVISOR CODE - CORE
//////////////////////////////////////////////////////////////////////

VMTrap CHyperVisor::HVEntryPoint( 
	__inout ULONG_PTR reg[REG_COUNT], 
	__in void* param 
	)
{
	m_callback(reg, param);

	VM_STATUS status;
	ULONG_PTR ExitReason = Instrinsics::VmRead(VMX_VMCS32_RO_EXIT_REASON, &status);
	if (VM_OK(status))
	{
		ULONG_PTR ExitInstructionLength = Instrinsics::VmRead(VMX_VMCS32_RO_EXIT_INSTR_LENGTH, &status);
		if (VM_OK(status))
		{
			ULONG_PTR GuestEIP = Instrinsics::VmRead(VMX_VMCS64_GUEST_RIP, &status);
			if (VM_OK(status))
			{
				status = Instrinsics::VmWrite(VMX_VMCS64_GUEST_RIP, GuestEIP + ExitInstructionLength);
				if (VM_OK(status))
				{
					if((ExitReason > VMX_EXIT_VMCALL) && (ExitReason <= VMX_EXIT_VMXON))
					{
						ULONG_PTR GuestRFLAGS = Instrinsics::VmRead(VMX_VMCS_GUEST_RFLAGS, &status);
						if (VM_OK(status))
							(void)Instrinsics::VmWrite(VMX_VMCS_GUEST_RFLAGS, GuestRFLAGS & (~0x8d5) | 0x1);
					}

					if (VMX_EXIT_CRX_MOVE == ExitReason)
						HandleCrxAccess(reg);

					return m_hvCallbacks[ExitReason];

				}
			}
		}
	}
	return DUMMY;
}

void CHyperVisor::HandleCrxAccess( 
	__inout ULONG_PTR reg[REG_COUNT] 
	)
{
	VM_STATUS status;
	ULONG_PTR ExitQualification = Instrinsics::VmRead(VMX_VMCS_RO_EXIT_QUALIFICATION, &status);
	if (VM_OK(status))
	{
		ULONG_PTR cr = (ExitQualification & 0x0000000F);
		ULONG_PTR operand = (ExitQualification & 0x00000040) >> 6;

		if (3 == cr && 0 == operand)
		{
			ULONG_PTR acess = (ExitQualification & 0x00000030) >> 4;
			ULONG_PTR r64 = (ExitQualification & 0x00000F00) >> 8;


			r64 = r64 < REG_X86_COUNT ? 
				((~r64) + REG_X86_COUNT) : 
			(REG_X86_COUNT + (~(r64 - REG_X86_COUNT)) + REG_X86_COUNT);

			if (1 == acess)
			{
				ULONG_PTR cr3 = Instrinsics::VmRead(VMX_VMCS64_GUEST_CR3, &status);
				if (VM_OK(status))
					reg[r64] = cr3;
			}
			else if (0 == acess)
			{
				(void)Instrinsics::VmWrite(VMX_VMCS64_GUEST_CR3, reg[r64]);

				//handle pagefault via VMX_EXIT_EPT_VIOLATION
				(void)Instrinsics::VmWrite(VMX_VMCS_CTRL_EPTP_FULL, reg[r64]);
				(void)Instrinsics::VmWrite(VMX_VMCS_CTRL_EPTP_HIGH, reg[r64] >> 32);
			}
		}
	}
}
	
//////////////////////////////////////////////////////////////////////
// STATIC HELPERS (PUBLIC)
//////////////////////////////////////////////////////////////////////

VMTrap CHyperVisor::DummyHandler()
{
	return DUMMY;
}

const void* CHyperVisor::HvExitPoint()
{
	return hv_exit;
}

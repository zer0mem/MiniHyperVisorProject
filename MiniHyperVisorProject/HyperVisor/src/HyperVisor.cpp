/**
 * @file HyperVisor.cpp
 * @author created by: Peter Hlavaty
 */

#include "StdAfx.h"

#include "HyperVisor.h"

CHyperVisor::CHyperVisor(__in BYTE coreId, __in_opt const ULONG_PTR traps[MAX_CALLBACK], __in_opt const VOID* callback) : m_coreId()
{
	m_callback = NULL != callback ? callback : (const VOID*)DUMMY;

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

//-- hypervisor core -- //
ULONG_PTR CHyperVisor::HVEntryPoint( __inout ULONG_PTR reg[0x10], __in VOID* param )
{
	(*(void (*)(ULONG_PTR*, const void*))(m_callback))(reg, param);

	ULONG_PTR ExitReason;
	vmread(VMX_VMCS32_RO_EXIT_REASON, &ExitReason);

	ULONG_PTR ExitInstructionLength;
	vmread(VMX_VMCS32_RO_EXIT_INSTR_LENGTH, &ExitInstructionLength);

	ULONG_PTR GuestEIP;
	vmread(VMX_VMCS64_GUEST_RIP, &GuestEIP);

	vmwrite(VMX_VMCS64_GUEST_RIP, GuestEIP + ExitInstructionLength);

	if((ExitReason > VMX_EXIT_VMCALL) && (ExitReason <= VMX_EXIT_VMXON))
	{
		ULONG_PTR GuestRFLAGS;
		vmread(VMX_VMCS_GUEST_RFLAGS, &GuestRFLAGS);
		vmwrite(VMX_VMCS_GUEST_RFLAGS, GuestRFLAGS & (~0x8d5) | 0x1);
	}

	if (VMX_EXIT_CRX_MOVE == ExitReason)
		HandleCrxAccess(reg);

	return m_hvCallbacks[ExitReason];
}

void CHyperVisor::HandleCrxAccess( __inout ULONG_PTR reg[0x10] )
{
	ULONG_PTR ExitQualification;
	vmread(VMX_VMCS_RO_EXIT_QUALIFICATION, &ExitQualification);

	ULONG_PTR cr = (ExitQualification & 0x0000000F);
	ULONG_PTR operand = (ExitQualification & 0x00000040) >> 6;

	if (3 == cr && 0 == operand)
	{
		ULONG_PTR acess = (ExitQualification & 0x00000030) >> 4;
		ULONG_PTR r64 = (ExitQualification & 0x00000F00) >> 8;

		if (1 == acess)
			vmread(VMX_VMCS64_GUEST_CR3, &reg[r64]);
		else if (0 == acess)
			vmwrite(VMX_VMCS64_GUEST_CR3, reg[r64]);
	}
}

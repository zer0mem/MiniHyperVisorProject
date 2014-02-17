/**
 * @file VMX.h
 * @author created by: Peter Hlavaty
 */

#ifndef __VMX_H__
#define __VMX_H__

#include "../Common/base/HVCommon.h"

class CVmx
{
public:
	CVmx(
		__in KAFFINITY procId,
		__in ULONG_PTR expcetionMask = 0
		);
	~CVmx();

	__checkReturn 
	bool InstallHyperVisor(
		__in const void* hvEntryPoint, 
		__in void* hvStack
		);
	
	__checkReturn
	VM_STATUS  VmcsToRing0();

	//force inline and public
	__checkReturn 
	bool CpuActivated() const
	{
		return m_cpuActivated;
	}
	
//static
	static
	void EnableVirtualization();

	static 
	__checkReturn 
	bool IsVirtualizationLocked();

	static
	__checkReturn
	bool IsVirtualizationEnabled();

protected:
	__checkReturn 
	VM_STATUS VmcsInit();

	__checkReturn 
	bool GetGuestState(
		__in KAFFINITY procId
		);

	void GetSegmentDescriptor(
		__out SEGMENT_SELECTOR* segSel, 
		__in ULONG_PTR selector
		);

	__checkReturn
	VM_STATUS SetSegSelector(
		__in ULONG_PTR segSelector,
		__in ULONG_PTR segField
		);

	__checkReturn
	VM_STATUS SetCRx();

	__checkReturn
	VM_STATUS SetControls();

	__checkReturn
	VM_STATUS SetDT();

	__checkReturn
	VM_STATUS SetSysCall();

protected:
	bool m_cpuActivated;
	ULONG_PTR m_expcetionMask;

private:
	bool m_preparedState;
	GUEST_STATE	m_guestState;
};

#endif //__VMX_H__

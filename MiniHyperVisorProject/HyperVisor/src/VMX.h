/**
 * @file VMX.h
 * @author created by: Peter Hlavaty
 */

#ifndef __VMX_H__
#define __VMX_H__

#include "../Common/base/HVCommon.h"

EXTERN_C VOID get_guest_exit(__out ULONG_PTR* guestRip, __out ULONG_PTR* guestRsp);

#define FEATURE_CONTROL_LOCKED			(1<<0)
#define FEATURE_CONTROL_VMXON_ENABLED	(1<<2)

class CVmx
{
public:
	CVmx(__in KAFFINITY procId);
	~CVmx();

	__checkReturn bool InstallHyperVisor(__in const VOID* hvEntryPoint, __in VOID* hvStack);
	__checkReturn bool CpuActivated() const { return m_cpuActivated; };

	static __checkReturn bool IsVirtualizationLocked();
	static __checkReturn bool IsVirtualizationEnabled();
	static void EnableVirtualization();

protected:
	__checkReturn bool VmcsInit();

	__checkReturn bool GetGuestState(__in KAFFINITY procId);

	void GetSegmentDescriptor(__out SEGMENT_SELECTOR* segSel, __in ULONG_PTR selector);
	void SetSegSelector(__in ULONG_PTR segSelector, __in ULONG_PTR segField);

	void SetCRx();
	void SetControls();
	void SetDT();
	void SetSysCall();

protected:
	bool m_cpuActivated;

private:
	bool m_preparedState;
	GUEST_STATE	m_guestState;
};

#endif //__VMX_H__

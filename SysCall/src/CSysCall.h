#pragma once

#include <ntifs.h>
#include <Common/base/DynamicClass.h>
#include <Common/utils/ProcessorWalker.hpp>
#include <HyperVisor/src/Cronos.h>

#include "../Common/Stack.hpp"

class CSysCall : 
	public CCRonos,
	public IExitCallback
{
public:
	CSysCall();
	~CSysCall() override;

	void
	Install();

	static 
	void* 
	GetSysCall(
		__in BYTE coreId
		);

	static 
	CStack& 
	GetRdmsrStack();

protected:
	static 
	void 
	HookProtectionMSR(
		__inout ULONG_PTR reg[0x10]
		);
	
	static 
	CStack 
	m_sRdmsrRips;


	virtual 
	void 
	PerCoreAction(
		__in BYTE coreId
		) override;

	virtual 
	__checkReturn 
	bool 
	SetVirtualizationCallbacks() override;

	void 
	HookSyscallMSR(
		__in const void* hook
		);

private:
	static void* m_syscalls[MAX_PROCID];
};


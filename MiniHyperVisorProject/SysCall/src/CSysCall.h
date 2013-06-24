/**
 * @file Syseneter.h
 * @author created by: Peter Hlavaty
 */

#ifndef __SYSENETER_H__
#define __SYSENETER_H__

#include "../../Common/base/Common.h"
#include "../../Common/utils/ProcessorWalker.hpp"

#include "../Common/Stack.hpp"
#include "../../Cronos/src/Cronos.h"

class CSysCall : public CCRonos
{
public:
	CSysCall();
	~CSysCall();

	void Install();

	static void* GetSysCall(__in BYTE coreId);

public:
	static CStack& GetRdmsrStack();

protected:
	static void HookProtectionMSR(__inout ULONG_PTR reg[0x10]);
	static CStack m_sRdmsrRips;


protected:
	virtual void PerCoreAction(__in BYTE coreId);
	virtual __checkReturn bool SetVirtualizationCallbacks();
	void HookSyscallMSR(__in const void* hook);

private:
	static void* m_syscalls[MAX_PROCID];
};

#endif //__SYSENETER_H__

/**
 * @file ColdPatcher.hpp
 * @author created by: Peter Hlavaty
 */

#ifndef __COLDPATCHER_H__
#define __COLDPATCHER_H__

#include "../Common/base/Common.h"
#include "../Kernel/IRQL.hpp"

class CColdPatcher
{
public:
	CColdPatcher(__in void* addr, __in void* hook) : m_addr(addr), m_hook((LONG64)hook), m_patched(false)
	{
		m_prolog = 0xB848;
		m_epilog = 0xD0FF;
		ReHook();
	}

	void ReHook()
	{
		m_patched = !m_patched;
		{
			CDisableInterrupts idis;
			m_prolog = InterlockedExchange16((SHORT*)m_addr, m_prolog);
			m_hook = InterlockedExchange64((LONG64*)((ULONG_PTR)m_addr + 2), m_hook);
			m_epilog = InterlockedExchange16((SHORT*)((ULONG_PTR)m_addr + sizeof(ULONG_PTR) + sizeof(m_prolog)), m_epilog);
		}
	}

	~CColdPatcher()
	{
		if (m_patched)
			ReHook();
	}

protected:
	bool m_patched;
	const void* m_addr;
	WORD m_prolog;
	WORD m_epilog;
	ULONG_PTR m_hook;
};

#endif //__COLDPATCHER_H__
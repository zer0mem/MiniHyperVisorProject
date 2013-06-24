/**
 * @file IRQL.hpp
 * @author created by: Peter Hlavaty
 */

#ifndef __IRQL_H__
#define __IRQL_H__

#include "../base/Common.h"
#include "../base/instrinsics.h"

class CDispatchLvl
{
public:
	CDispatchLvl()
	{
		KeRaiseIrql(DISPATCH_LEVEL, &m_oldIRQL);
	}

	~CDispatchLvl()
	{
		KeLowerIrql(m_oldIRQL);
	}
private:
	KIRQL m_oldIRQL;
};

class CPassiveLvl
{
public:
	CPassiveLvl()
	{
		KeRaiseIrql(PASSIVE_LEVEL, &m_oldIRQL);
	}

	~CPassiveLvl()
	{
		KeLowerIrql(m_oldIRQL);
	}
private:
	KIRQL m_oldIRQL;
};

class CDisableInterrupts
{
public:
	CDisableInterrupts()
	{
		cli();
	}

	~CDisableInterrupts()
	{
		sti();
	}
};

#endif //__IRQL_H__
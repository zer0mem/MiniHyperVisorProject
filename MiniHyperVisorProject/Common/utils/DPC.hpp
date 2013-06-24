/**
 * @file DPC.hpp
 * @author created by: Peter Hlavaty
 */

#ifndef __DPC_H__
#define __DPC_H__

#include "../Common/base/Common.h"
#include "../Kernel/IRQL.hpp"

class CDPC
{
public:
	CDPC(
		__in_opt bool notificationType = false
		)
	{
		m_interval = 0;
		m_expireTime.QuadPart = -1;
		m_tolerableDelay = 0;

		KeInitializeDpc(&m_timerDpc, (PKDEFERRED_ROUTINE)StaticDPC, this);
		{
			CDispatchLvl auto_irql;
			KeInitializeTimerEx(&m_timer, notificationType ? NotificationTimer : SynchronizationTimer);
		}
	}

	~CDPC()
	{
		StopTimer();
		m_interval = 0;
	}

	void ResetTimer(
		__in_opt size_t expireTime,
		__in ULONG interval = 0,
		__in ULONG tolerableDelay = 0,
		__in_opt bool relative = true)
	{
		StopTimer();
		m_interval = interval;
		m_expireTime.QuadPart = relative ? (~expireTime) : expireTime;
	}

	void KeepTimerInLoop()
	{
		if (m_interval || m_expireTime.QuadPart != -1)
		{
			CDispatchLvl auto_irql;
			KeSetCoalescableTimer(&m_timer, m_expireTime, m_interval, m_tolerableDelay, &m_timerDpc);
		}
	}

	void StopTimer()
	{
		if (m_interval)
		{			
			m_interval = 0;
			{
				CDispatchLvl auto_irql;
				KeCancelTimer(&m_timer);
			}
		}
	}

	virtual void CustomDPC(
		__in struct _KDPC *Dpc, 
		__in void* DeferredContext, 
		__in_opt void* SystemArgument1, 
		__in_opt void* SystemArgument2) = 0;

private:
	static void StaticDPC(
		__in struct _KDPC* Dpc, 
		__in void* DeferredContext, 
		__in_opt void* SystemArgument1, 
		__in_opt void* SystemArgument2
		)
	{
		CDPC* dpc = (CDPC*)DeferredContext;
		dpc->CustomDPC(Dpc, DeferredContext, SystemArgument1, SystemArgument2);
	}

protected:
	ULONG m_interval;
	LARGE_INTEGER m_expireTime;
	ULONG m_tolerableDelay;
	KTIMER m_timer;

private:
	KDPC m_timerDpc;
};

class COneLoopDPC : public CDPC
{
public:
	COneLoopDPC(
		__in ULONG expireTime
		) : CDPC(false)
	{
		ResetTimer(expireTime);
	}
};

class CIntervalDPC : public CDPC
{
public:
	CIntervalDPC(
		__in ULONG interval
		) : CDPC(false)
	{
		ResetTimer(0, interval);
	}
};

#endif //__DPC_H__

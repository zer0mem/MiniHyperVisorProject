/**
 * @file Stack.hpp
 * @author created by: Peter Hlavaty
 */

#ifndef __STACK_H__
#define __STACK_H__

//missing locks ;)
class CStack
{
public:
	CStack() : m_top(0), m_bottom(0), m_alert(false)
	{
	}

	__checkReturn bool IsEmpty()
	{
		return (m_bottom == m_top);
	}

	ULONG_PTR Push(__in ULONG_PTR val)
	{
		if (m_bottom <= m_top)
		{
			m_alert = false;
			size_t ind = InterlockedExchangeAdd64((LONG64*)&m_top, 1);
			m_readMsrAttempts[ind] = val;
		}
		else if (!m_alert)
		{
			m_alert = true;
			DbgPrint("\n ... stack is full ...\n");
		}
		return val;
	}

	ULONG_PTR Pop()
	{
		size_t ind = InterlockedExchangeAdd64((LONG64*)&m_bottom, 1);
		return m_readMsrAttempts[ind];
	}
protected:
	bool m_alert;
	size_t m_top;
	size_t m_bottom;
	ULONG_PTR m_readMsrAttempts[0x100];
};

#endif //__STACK_H__

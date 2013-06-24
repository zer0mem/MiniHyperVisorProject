/**
 * @file ProcessorWalker.hpp
 * @author created by: Peter Hlavaty
 */

#ifndef __PROCESSORWALKER_H__
#define __PROCESSORWALKER_H__

#include "../base/Common.h"

class CProcessorWalker
{
public:
	CProcessorWalker()
	{
		m_cpuCount = (BYTE)KeQueryActiveProcessorCount(&m_cpu);
	}

	__checkReturn bool NextCore(__inout BYTE* coreId, __in_opt BYTE startId = 0)
	{
		ULONG n_cpu = 0;
		if (startId < (sizeof(ULONG) << 3))
		{
			if (PROCID(startId) & m_cpu)
			{
				*coreId = startId;
				return true;
			}

			startId++;
		}

		return false;
	}

protected:
	KAFFINITY m_cpu;
	BYTE m_cpuCount;
};

#endif //__PROCESSORWALKER_H__
/**
 * @file Mdl.cpp
 * @author created by: Peter Hlavaty
 */

#include "StdAfx.h"
#include "Mdl.h"

CMdl::CMdl(__in const void* virtualAddress, __in size_t size) : m_locked(false), m_mem(NULL), m_mdl(NULL)
{
	m_mdl = IoAllocateMdl((PVOID)virtualAddress, (ULONG)size, FALSE, FALSE, NULL);
}

CMdl::~CMdl()
{
	if (m_mdl)
	{
		//call also CMdl::Unmap() ?
		MmUnlockPages(m_mdl);
		IoFreeMdl(m_mdl);
	}
}

__checkReturn bool CMdl::Lock( __in_opt LOCK_OPERATION operation /*= IoReadAccess*/ )
{
	if (!m_mdl)
		return false;

	__try 
	{
		MmProbeAndLockPages(m_mdl, KernelMode, operation);
		m_locked = true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
	return true;
}

void* CMdl::Map( __in_opt MEMORY_CACHING_TYPE cacheType /*= MmCached*/ )
{
	if (!m_mdl)//mdl fail
		return NULL;

	if (m_mem)//need to unmap first
		return NULL;
	
	if (!m_locked && !Lock())
		return NULL;

	__try 
	{
		m_mem = MmMapLockedPagesSpecifyCache(m_mdl, KernelMode, cacheType, NULL, FALSE, NormalPagePriority);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	return m_mem;
}

void CMdl::Unmap()
{
	if (m_mem && m_mdl)
	{
		MmUnmapLockedPages(m_mem, m_mdl);
		m_mem = NULL;
	}
}

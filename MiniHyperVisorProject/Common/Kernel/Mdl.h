/**
 * @file Mdl.h
 * @author created by: Peter Hlavaty
 */

#ifndef __MDL_H__
#define __MDL_H__

#include "../base/Common.h"
#include "IRQL.hpp"

//this object work on DISPATCH_LEVEL. Dont change its IRQL during working with this object!
//suposed to use just for quick map & copy/write data ...
class CMdl
{
public:
	CMdl(__in const void* virtualAddress, __in size_t size);
	~CMdl();

	__checkReturn bool Lock(__in_opt LOCK_OPERATION operation = IoReadAccess);

	void* Map(__in_opt MEMORY_CACHING_TYPE cacheType = MmCached);
	void Unmap();

protected:
	MDL* m_mdl;
	void* m_mem;
	bool m_locked;
	CDispatchLvl m_DispatchIRQL;
};

#endif //__MDL_H__

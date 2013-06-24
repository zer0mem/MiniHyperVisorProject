#include "StdAfx.h"

#ifndef _LIBC_POOL_TAG
#define _LIBC_POOL_TAG	'colM'
#endif

typedef struct _MEMBLOCK
{
	size_t	size;
	
	char	data[0];
} MEMBLOCK;

EXTERN_C
__drv_when(return!=0, __drv_allocatesMem(Mem))
__checkReturn
__drv_maxIRQL(DISPATCH_LEVEL)
__bcount_opt(Size)
void * __cdecl malloc(__in size_t Size)
{
	MEMBLOCK *pBlock = (MEMBLOCK*)ExAllocatePoolWithTag(NonPagedPool, Size + sizeof(MEMBLOCK), _LIBC_POOL_TAG);
	if (NULL == pBlock)
		return NULL;

	pBlock->size = Size;
	
	return pBlock->data;
}

EXTERN_C
__drv_when(return!=0, __drv_allocatesMem(Mem))
__checkReturn
__drv_maxIRQL(DISPATCH_LEVEL)
__bcount_opt(size)
void * __cdecl realloc(__in_opt void *ptr, __in size_t size)
{
	MEMBLOCK *inblock = (NULL != ptr) ? CONTAINING_RECORD(ptr, MEMBLOCK, data) : NULL;

	if (0 == size)
	{
		// freeing all
		if (NULL != inblock)
			ExFreePoolWithTag(inblock, _LIBC_POOL_TAG);
		return NULL;
	}

	// alloc new block
	MEMBLOCK *outblock = (MEMBLOCK*)ExAllocatePoolWithTag(NonPagedPool, size + sizeof(MEMBLOCK), _LIBC_POOL_TAG);
	if (NULL == outblock)
		return NULL;

	outblock->size = size;

	if (NULL != inblock)
	{
		// copy from old one
		memcpy(outblock->data, inblock->data, min(inblock->size, outblock->size));
		// and then free it
		ExFreePoolWithTag(inblock, _LIBC_POOL_TAG);
	}
	return outblock->data;
}

EXTERN_C
__drv_maxIRQL(DISPATCH_LEVEL)
void __cdecl free(__inout_opt __drv_freesMem(Mem) void *ptr)
{
	if (NULL != ptr)
		ExFreePoolWithTag(CONTAINING_RECORD(ptr, MEMBLOCK, data), _LIBC_POOL_TAG);
}

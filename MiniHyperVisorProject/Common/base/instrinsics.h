/**
 * @file instrinsics.h
 * @author created by: Peter Hlavaty
 */

#ifndef __INSTRINSICS_H__
#define __INSTRINSICS_H__

#include <intrin.h>
#include "Common.h"

EXTERN_C ULONG_PTR __rol(ULONG_PTR val, BYTE rotation);
EXTERN_C ULONG_PTR __ror(ULONG_PTR val, BYTE rotation);

EXTERN_C void __cli();
EXTERN_C void __sti();

EXTERN_C ULONG_PTR __str();
EXTERN_C ULONG_PTR __sldt();
EXTERN_C ULONG_PTR __sgdt(__out void * gdtr);
EXTERN_C ULONG_PTR __vmx_call(__in ULONG_PTR);

EXTERN_C ULONG_PTR __readcs();
EXTERN_C ULONG_PTR __readds();
EXTERN_C ULONG_PTR __reades();
EXTERN_C ULONG_PTR __readss();
EXTERN_C ULONG_PTR __readfs();
EXTERN_C ULONG_PTR __readgs();

EXTERN_C ULONG_PTR __xchgds(__inout ULONG_PTR* ds);
EXTERN_C ULONG_PTR __writeds(__in ULONG_PTR ds);

#define rdmsr(reg)				__readmsr(reg)
#define wrmsr(reg,val64)		__writemsr(reg, val64)

#define ROL(val, rotation)		__rol(val, rotation)
#define ROR(val, rotation)		__ror(val, rotation)

#define cli()					__cli()
#define sti()					__sti()
#define sgdt(dtp)				__sgdt(dtp)
#define sidt(dtp)				__sidt(dtp)
#define str()					__str()
#define sldt()					__sldt()
#define readcr0()				__readcr0()
#define readcr3()				__readcr3()
#define readcr4()				__readcr4()
#define readeflags()			__readeflags()
#define writecr0(cr0)			__writecr0(cr0)
#define writecr4(cr4)			__writecr4(cr4)
#define	rdtsc()					__rdtsc()

#define vmread(field, pval)		__vmx_vmread(field, (size_t*)pval)
#define vmwrite(field, val)		__vmx_vmwrite(field, (size_t)val)

#define vmxon(vmcs)				__vmx_on((ULONG_PTR*)vmcs)
#define vmclear(vmcs)			__vmx_vmclear((ULONG_PTR*)vmcs)
#define vmptrld(vmcs)			__vmx_vmptrld((ULONG_PTR*)vmcs)
#define vmlaunch()				__vmx_vmlaunch()
#define vmresume()				__vmx_vmresume()
#define vmxoff()				__vmx_off()
#define vmcall(magic)			__vmx_call(magic)

#define readcs()				__readcs()
#define readds()				__readds()
#define reades()				__reades()
#define readss()				__readss()
#define readfs()				__readfs()
#define readgs()				__readgs()

#define xchgds(ds)				__xchgds(ds)
#define writeds(ds)				__writeds(ds)

#endif //__INSTRINSICS_H__

#include "../drv_common.h"
#include "../../Common/base/Common.h"

#include "./src/CSysCall.h"

CSysCall gSysCall;

EXTERN_C 
int __cdecl _cinit(
	__in int
	);

EXTERN_C
void __cdecl doexit(
	__in int /*code*/,
	__in int /*quick*/,
	__in int /*retcaller*/
	);

//---------------------------
//-----   UNINSTALL   -------
//---------------------------
void OnUnload(
	__in DRIVER_OBJECT* DriverObject
	)
{
	UNREFERENCED_PARAMETER(DriverObject);

	doexit(0, 0, 0);

	DbgPrint("\n\n************** unloaded! <<<<<<<<<<<<<<<\n\n");
} // end OnUnload

//---------------------------
//------   INSTALL   --------
//---------------------------
NTSTATUS DriverEntry(
	__in DRIVER_OBJECT* driverObject,
	__in UNICODE_STRING* RegistryPath
	)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

	_cinit(0);

	gSysCall.Install();

	driverObject->DriverUnload = OnUnload;

	return STATUS_SUCCESS;
} // end DriverEntry()

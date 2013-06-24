#include "StdAfx.h"

#include "./src/CSysCall.h"

PDRIVER_OBJECT DriverObject;

CSysCall* gSC;

//not properly implemented; i just dont have a time and passion to handle with : 
//error LNK2001: unresolved external symbol "void __cdecl operator delete(void *)"
void OnUnload(__in DRIVER_OBJECT* DriverObject)
{
	if (gSC)
	{
		//gSC->~CSysCall();
		free(gSC);
	}
}

NTSTATUS DriverEntry(__in DRIVER_OBJECT* driverObject, __in UNICODE_STRING* RegistryPath)
{
	DbgPrint("DriverEntry\n");
	driverObject->DriverUnload = OnUnload;

	gSC = (CSysCall*)malloc(sizeof(*gSC));
	if (gSC)
	{
		::new(gSC) CSysCall();

		gSC->Install();
	}

    return STATUS_SUCCESS;
} // end DriverEntry()

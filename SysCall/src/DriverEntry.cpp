#include <ntifs.h>
#include <memory>

#include <Common/kernel/KernelModule.hpp>
#include <SysCall/src/CSysCall.h>

//---------------------------
//----   ENTRY PIONT   ------
//---------------------------


__checkReturn
bool
drv_main()
{
	printf("\n ENTRY POINT - enter \n");

	auto hyper_test = new CSysCall;
	if (!hyper_test)
		return false;

	CKernelModule::GetInstance().RegisterRuntimeClass(*hyper_test);
	hyper_test->Install();

	printf("\n ENTRY POINT - exit \n");
	return true;
}

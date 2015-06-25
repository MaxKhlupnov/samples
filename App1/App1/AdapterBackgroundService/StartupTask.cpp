#include "pch.h"
#include "StartupTask.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace AdapterBackgroundService;

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
    deferral = taskInstance->GetDeferral();

    try
    {
        AdapterLib::Adapter^ adapter = ref new AdapterLib::Adapter();
        dsbBridge = ref new BridgeRT::DsbBridge(adapter);
        if (this->dsbBridge == nullptr)
        {
            throw ref new Exception(
                HRESULT_FROM_WIN32(ERROR_NOT_READY),
                "DSB Bridge not instantiated!");
        }

        HRESULT hr = this->dsbBridge->Initialize();
        if (FAILED(hr))
        {
            throw  ref new Exception(hr, "DSB Bridge initialization failed!");
        }
    }
    catch (Exception^ ex)
    {
        deferral->Complete();
        throw ref new Exception(E_FAIL, "Exception occurred during the execution of the DSB Bridge!");
    }
}
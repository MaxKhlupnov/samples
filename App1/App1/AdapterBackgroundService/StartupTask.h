#pragma once

#include "pch.h"

using namespace Windows::ApplicationModel::Background;
using namespace Windows::System::Threading;

namespace AdapterBackgroundService
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class StartupTask sealed : public IBackgroundTask
    {
    public:
        virtual void Run(IBackgroundTaskInstance^ taskInstance);
    private:
        BridgeRT::DsbBridge^ dsbBridge;
        Platform::Agile<Windows::ApplicationModel::Background::BackgroundTaskDeferral> deferral;
    };
}
//
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Module Name:
//
//      Adapter.cpp
//
// Abstract:
//
//      Adapter class implementation.
//
//      Adapter class implements the IAdapter interface.
//      When the DSB bridge component uses the adapter it instantiates an Adapter object 
//      and uses it as the IAdapter.
//
//
#include "pch.h"
#include "Adapter.h"
#include "AdapterDevice.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;

using namespace BridgeRT;
using namespace DsbCommon;

namespace AdapterLib
{

    Adapter::Adapter()
    {
        this->vendor = L"Custom Vendor";
        this->adapterName = L"Custom Adapter";
        this->version = L"1.0.0.1";
    }


    Adapter::~Adapter()
    {
    }


    _Use_decl_annotations_
    uint32
    Adapter::SetConfiguration(const Platform::Array<byte>^ ConfigurationData)
    {
        UNREFERENCED_PARAMETER(ConfigurationData);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::GetConfiguration(Platform::Array<byte>^* ConfigurationDataPtr)
    {
        UNREFERENCED_PARAMETER(ConfigurationDataPtr);

        return ERROR_SUCCESS;
    }


    uint32
    Adapter::Initialize()
    {
        return ERROR_SUCCESS;
    }


    uint32
    Adapter::Shutdown()
    {
        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::EnumDevices(
        ENUM_DEVICES_OPTIONS Options,
        IAdapterDeviceVector^* DeviceListPtr,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Options);
        UNREFERENCED_PARAMETER(DeviceListPtr);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::GetProperty(
        IAdapterProperty^ Property,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::SetProperty(
        IAdapterProperty^ Property,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::GetPropertyValue(
        IAdapterProperty^ Property,
        String^ AttributeName,
        IAdapterValue^* ValuePtr,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(AttributeName);
        UNREFERENCED_PARAMETER(ValuePtr);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::SetPropertyValue(
        IAdapterProperty^ Property,
        IAdapterValue^ Value,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(Value);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::CallMethod(
        IAdapterMethod^ Method,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Method);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::RegisterSignalListener(
        IAdapterSignal^ Signal,
        IAdapterSignalListener^ Listener,
        Object^ ListenerContext
        )
    {
        UNREFERENCED_PARAMETER(Signal);
        UNREFERENCED_PARAMETER(Listener);
        UNREFERENCED_PARAMETER(ListenerContext);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::UnregisterSignalListener(
        IAdapterSignal^ Signal,
        IAdapterSignalListener^ Listener
        )
    {
        UNREFERENCED_PARAMETER(Signal);
        UNREFERENCED_PARAMETER(Listener);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::NotifySignalListener(
        IAdapterSignal^ Signal
        )
    {
        UNREFERENCED_PARAMETER(Signal);

        return ERROR_SUCCESS;
    }


    uint32
    Adapter::createSignals()
    {
        return ERROR_SUCCESS;
    }
} // namespace AdapterLib
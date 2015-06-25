//
// Copyright (c) 2015, Microsoft Corporation
// 
// Permission to use, copy, modify, and/or distribute this software for any 
// purpose with or without fee is hereby granted, provided that the above 
// copyright notice and this permission notice appear in all copies.
// 
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES 
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
// IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

#pragma once

#include <collection.h>
#include <algorithm>

namespace BridgeRT
{
    //
    // Adapter objects vector/vector interfaces
    //

    interface class IAdapterValue;
    typedef Platform::Collections::Vector<IAdapterValue^> AdapterValueVector;
    typedef Windows::Foundation::Collections::IVector<IAdapterValue^> IAdapterValueVector;
    typedef Windows::Foundation::PropertyType AdapterValueType;

    interface class IAdapterProperty;
    typedef Platform::Collections::Vector<IAdapterProperty^> AdapterPropertyVector;
    typedef Windows::Foundation::Collections::IVector<IAdapterProperty^> IAdapterPropertyVector;

    interface class IAdapterMethod;
    typedef Platform::Collections::Vector<IAdapterMethod^> AdapterMethodVector;
    typedef Windows::Foundation::Collections::IVector<IAdapterMethod^> IAdapterMethodVector;

    interface class IAdapterSignal;
    typedef Platform::Collections::Vector<IAdapterSignal^> AdapterSignalVector;
    typedef Windows::Foundation::Collections::IVector<IAdapterSignal^> IAdapterSignalVector;

    interface class IAdapterDevice;
    typedef Platform::Collections::Vector<IAdapterDevice^> AdapterDeviceVector;
    typedef Windows::Foundation::Collections::IVector<IAdapterDevice^> IAdapterDeviceVector;


    //
    // Adapter level signals and parameters names
    //

    // Device arrival signal
    static Platform::String^ DEVICE_ARRIVAL_SIGNAL = ref new Platform::String(L"Device_Arrival");
    static Platform::String^ DEVICE_ARRIVAL__DEVICE_HANDLE = ref new Platform::String(L"Device_Handle");
    
    // Device removal signal
    static Platform::String^ DEVICE_REMOVAL_SIGNAL = ref new Platform::String(L"Device_Removal");
    static Platform::String^ DEVICE_REMOVAL__DEVICE_HANDLE = ref new Platform::String(L"Device_Handle");

    // Change Of Value signal
    static Platform::String^ CHANGE_OF_VALUE_SIGNAL = ref new Platform::String(L"Change_Of_Value");
    static Platform::String^ COV__PROPERTY_HANDLE = ref new Platform::String(L"Property_Handle");
    static Platform::String^ COV__ATTRIBUTE_HANDLE = ref new Platform::String(L"Attribute_Handle");
}
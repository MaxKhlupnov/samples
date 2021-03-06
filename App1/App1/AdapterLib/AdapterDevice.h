//
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Module Name:
//
//      AdapterDevice.h
//
// Abstract:
//
//      AdapterValue, AdapterProperty, AdapterSignal, and AdapterDevice classes declaration.
//
//      Together with the Adapter class, the classes declared in this file implement the IAdapter interface.
//
//
#pragma once

#include "AdapterDefinitions.h"

namespace AdapterLib
{
    //
    // AdapterValue.
    // Description:
    // The class that implements BridgeRT::IAdapterValue.
    //
    ref class AdapterValue : BridgeRT::IAdapterValue
    {
    public:
        //
        // Generic for Adapter objects
        //
        virtual property Platform::String^ Name
        {
            Platform::String^ get() { return this->name; }
        }
        
        // Data
        virtual property Platform::Object^ Data
        {
            Platform::Object^ get() { return this->data; }
            void set(Platform::Object^ NewData) { this->data = NewData; }
        }

    internal:
        AdapterValue(
            Platform::String^ ObjectName,
            Platform::Object^ DefaultData = nullptr // For signature initialization
            );
        AdapterValue(const AdapterValue^ Other);

        uint32 Set(BridgeRT::IAdapterValue^ Other);

    private:
        // Generic
        Platform::String^ name;
        Platform::Object^ data;
    };


    //
    // AdapterProperty.
    // Description:
    // The class that implements BridgeRT::IAdapterProperty.
    //
    ref class AdapterProperty : BridgeRT::IAdapterProperty
    {
    public:
        //
        // Generic for Adapter objects
        //
        virtual property Platform::String^ Name
        {
            Platform::String^ get() { return this->name; }
        }
        
        // Attributes
        virtual property BridgeRT::IAdapterValueVector^ Attributes
        {
            BridgeRT::IAdapterValueVector^ get()
            {
                return ref new BridgeRT::AdapterValueVector(this->attributes);
            }
        }

    internal:
        AdapterProperty(Platform::String^ ObjectName);
        AdapterProperty(const AdapterProperty^ Other);

        uint32 Set(BridgeRT::IAdapterProperty^ Other);

        // Adding Attributes
        AdapterProperty^ operator += (BridgeRT::IAdapterValue^ Attribute)
        {
            this->attributes.push_back(Attribute);
            return this;
        }

    private:
        // Generic
        Platform::String^ name;
        
        std::vector<BridgeRT::IAdapterValue^> attributes;
    };


    //
    // AdapterMethod.
    // Description:
    // The class that implements BridgeRT::IAdapterMethod.
    //
    ref class AdapterMethod : BridgeRT::IAdapterMethod
    {
    public:
        // Object name
        virtual property Platform::String^ Name
        {
            Platform::String^ get() { return this->name; }
        }

        // Method description
        virtual property Platform::String^ Description
        {
            Platform::String^ get() { return this->description; }
        }

        // The input parameters
        virtual property BridgeRT::IAdapterValueVector^ InputParams
        {
            BridgeRT::IAdapterValueVector^ get()
            {
                return ref new BridgeRT::AdapterValueVector(this->inParams);
            }
            void set(BridgeRT::IAdapterValueVector^ Params);
        }

        // The output parameters
        virtual property BridgeRT::IAdapterValueVector^ OutputParams
        {
            BridgeRT::IAdapterValueVector^ get()
            {
                return ref new BridgeRT::AdapterValueVector(this->outParams);
            }
        }

        // The return value
        virtual property int32 HResult
        {
            int32 get() { return this->result; }
        }

    internal:
        AdapterMethod(Platform::String^ ObjectName);
        AdapterMethod(const AdapterMethod^ Other);

        // Adding parameters
        void AddInputParam(BridgeRT::IAdapterValue^ InParameter)
        {
            this->inParams.push_back(InParameter);
        }
        void AddOutputParam(BridgeRT::IAdapterValue^ OutParameter)
        {
            this->outParams.push_back(OutParameter);
        }

        void SetResult(HRESULT Hr);

    private:
        // Generic
        Platform::String^ name;
        
        // Method information
        Platform::String^ description;

        // Method parameters
        std::vector<BridgeRT::IAdapterValue^> inParams;
        std::vector<BridgeRT::IAdapterValue^> outParams;
        int32 result;
    };


    //
    // AdapterSignal.
    // Description:
    // The class that implements BridgeRT::IAdapterSignal.
    //
    ref class AdapterSignal : BridgeRT::IAdapterSignal
    {
    public:
        //
        // Generic for Adapter objects
        //
        virtual property Platform::String^ Name
        {
            Platform::String^ get() { return this->name; }
        }
        
        // Signal parameters
        virtual property BridgeRT::IAdapterValueVector^ Params
        {
            BridgeRT::IAdapterValueVector^ get()
            {
                return ref new BridgeRT::AdapterValueVector(this->params);
            }
        }

    internal:
        AdapterSignal(Platform::String^ ObjectName);
        AdapterSignal(const AdapterSignal^ Other);

        // Adding signal parameters
        AdapterSignal^ operator += (BridgeRT::IAdapterValue^ Parameter)
        {
            this->params.push_back(Parameter);
            return this;
        }

    private:
        // Generic
        Platform::String^ name;
        
        std::vector<BridgeRT::IAdapterValue^> params;
    };


    //
    // DEVICE_DESCRIPTOR
    // Description:
    // A device descriptor.
    //
    struct DEVICE_DESCRIPTOR
    {
        // The device name
        Platform::String^           Name;

        // The device manufacturer name
        Platform::String^           VendorName;

        // The device model name
        Platform::String^           Model;

        // The device version number
        Platform::String^           Version;

        // The device serial number
        Platform::String^           SerialNumer;

        // The specific device description
        Platform::String^           Description;
    };


    //
    // AdapterDevice.
    // Description:
    // The class that implements BridgeRT::IAdapterDevice.
    //
    ref class AdapterDevice : BridgeRT::IAdapterDevice
    {
    public:
        //
        // Generic for Adapter objects
        //
        virtual property Platform::String^ Name
        {
            Platform::String^ get() { return this->name; }
        }
        
        //
        // Device information
        //
        virtual property Platform::String^ Vendor
        {
            Platform::String^ get() { return this->vendor; }
        }
        virtual property Platform::String^ Model
        {
            Platform::String^ get() { return this->model; }
        }
        virtual property Platform::String^ Version
        {
            Platform::String^ get() { return this->firmwareVersion; }
        }
        virtual property Platform::String^ FirmwareVersion
        {
            Platform::String^ get() { return this->firmwareVersion; }
        }
        virtual property Platform::String^ ID
        {
            Platform::String^ get() { return this->serialNumber; }
        }
        virtual property Platform::String^ SerialNumber
        {
            Platform::String^ get() { return this->serialNumber; }
        }
        virtual property Platform::String^ Description
        {
            Platform::String^ get() { return this->description; }
        }

        // Device properties
        virtual property BridgeRT::IAdapterPropertyVector^ Properties
        {
            BridgeRT::IAdapterPropertyVector^ get()
            {
                return ref new BridgeRT::AdapterPropertyVector(this->properties);
            }
        }

        // Device methods
        virtual property BridgeRT::IAdapterMethodVector^ Methods
        {
            BridgeRT::IAdapterMethodVector^ get()
            {
                return ref new BridgeRT::AdapterMethodVector(this->methods);
            }
        }

        // Device signals
        virtual property BridgeRT::IAdapterSignalVector^ Signals
        {
            BridgeRT::IAdapterSignalVector^ get()
            {
                return ref new BridgeRT::AdapterSignalVector(this->signals);
            }
        }

    internal:
		AdapterDevice(Platform::String^ ObjectName);
        AdapterDevice(const DEVICE_DESCRIPTOR* DeviceDescPtr);
        AdapterDevice(const AdapterDevice^ Other);

        // Adding Properties
        void AddProperty(BridgeRT::IAdapterProperty^ Property)
        {
            this->properties.push_back(Property);
        }

        // Adding Methods
        void AddMethod(BridgeRT::IAdapterMethod^ Method)
        {
            this->methods.push_back(Method);
        }

        // Adding Signals
        void AddSignal(BridgeRT::IAdapterSignal^ Signal)
        {
            this->signals.push_back(Signal);
        }

    private:
        // Generic
        Platform::String^ name;
        
        // Device information
        Platform::String^ vendor;
        Platform::String^ model;
        Platform::String^ firmwareVersion;
        Platform::String^ serialNumber;
        Platform::String^ description;

        // Device properties 
        std::vector<BridgeRT::IAdapterProperty^> properties;

        // Device methods
        std::vector<BridgeRT::IAdapterMethod^> methods;

        // Device signals 
        std::vector<BridgeRT::IAdapterSignal^> signals;
    };
} // namespace AdapterLib
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

#include "pch.h"
#include "DsbServiceNames.h"
#include "Bridge.h"
#include "BridgeDevice.h"
#include "PropertyInterface.h"
#include "AllJoynProperty.h"

using namespace BridgeRT;
using namespace std;

static std::string EMIT_CHANGE_SIGNAL_ANNOTATION = "org.freedesktop.DBus.Property.EmitsChangedSignal";
static std::string TRUE_VALUE = "true";

PropertyInterface::PropertyInterface()
    : m_interfaceDescription(NULL),
    m_indexForAJProperty(1)
{
}

PropertyInterface::~PropertyInterface()
{
    for (auto ajProperty : m_AJProperties)
    {
        delete ajProperty;
    }
    m_AJProperties.clear();
}

QStatus PropertyInterface::Create(IAdapterProperty ^adapterProperty, _In_ string &name, BridgeDevice ^parent)
{
    QStatus status = ER_OK;
    string tempName;
    AllJoynProperty *ajProperty = nullptr;

    // sanity check
    if (nullptr == adapterProperty)
    {
        status = ER_BAD_ARG_1;
        goto leave;
    }
    if (0 == name.length())
    {
        status = ER_BAD_ARG_2;
        goto leave;
    }
    if (nullptr == parent)
    {
        status = ER_BAD_ARG_3;
        goto leave;
    }

    m_interfaceName = name; 
    m_parent = parent;

    // create alljoyn interface 
    // note that the interface isn't suppose to already exist => ER_BUS_IFACE_ALREADY_EXISTS is an error
    if (DsbBridge::SingleInstance()->GetConfigManager()->IsDeviceAccessSecured())
    {
        status = alljoyn_busattachment_createinterface_secure(m_parent->GetBusAttachment(), m_interfaceName.c_str(), &m_interfaceDescription, AJ_IFC_SECURITY_REQUIRED);
    }
    else
    {
        status = alljoyn_busattachment_createinterface(m_parent->GetBusAttachment(), m_interfaceName.c_str(), &m_interfaceDescription);
    }
    if (ER_OK != status)
    {
        return status;
    }

    // create alljoyn properties from attributes of adapter property
    for (auto adapterAttribute : adapterProperty->Attributes)
    {
        // create new property for AllJoyn
        ajProperty = new(std::nothrow) AllJoynProperty();
        if (nullptr == ajProperty)
        {
            status = ER_OUT_OF_MEMORY;
            goto leave;
        }

        status = ajProperty->Create(adapterAttribute, this);
        if (ER_OK != status)
        {
            goto leave;
        }

        // expose property on alljoyn
        status = alljoyn_interfacedescription_addproperty(m_interfaceDescription, 
            ajProperty->GetName()->c_str(), 
            ajProperty->GetSignature()->c_str(), 
            ALLJOYN_PROP_ACCESS_RW);
        if (ER_OK != status)
        {
            goto leave;
        }
        // add change signal annotation to property if
        // device support COV signal (Change Of Value)
        //
        // note: 
        // COV signal is at adapter device level hence either supported for all interfaces
        // or not supported by any interface
        if (parent->IsCOVSupported())
        {
            status = alljoyn_interfacedescription_addpropertyannotation(m_interfaceDescription,
                ajProperty->GetName()->c_str(),
                EMIT_CHANGE_SIGNAL_ANNOTATION.c_str(),
                TRUE_VALUE.c_str());
            if (ER_OK != status)
            {
                goto leave;
            }
        }

        // add property in list
        m_AJProperties.push_back(ajProperty);
        ajProperty = nullptr;
    }

    alljoyn_interfacedescription_activate(m_interfaceDescription);

leave:
    if (ER_OK != status &&
        nullptr != ajProperty)
    {
        delete ajProperty;
    }
    return status;
}

bool PropertyInterface::InterfaceMatchWithAdapterProperty(IAdapterProperty ^adapterProperty)
{
    bool retVal = false;
    vector <IAdapterValue ^> tempList;

    // create temporary list of IAdapterValue that have to match with one of the 
    // AllJoyn properties
    for (auto adapterValue : adapterProperty->Attributes)
    {
        tempList.push_back(adapterValue);
    }

    // go through AllJoyn properties and find matching IAdapterValue
    for (auto ajProperty : m_AJProperties)
    {
        retVal = false;
        auto adapterValue = tempList.end();
        for (adapterValue = tempList.begin(); adapterValue != tempList.end(); adapterValue++)
        {
            if (ajProperty->IsSameType(*adapterValue))
            {
                retVal = true;
                break;
            }
        }
        if (retVal)
        {
            // remove adapterValue from temp list
            tempList.erase(adapterValue);
        }
        else
        {
            // interface doesn't match
            break;
        }
    }

    return retVal;
}

bool PropertyInterface::IsAJPropertyNameUnique(std::string name)
{
    bool retval = true;

    for (auto ajProperty : m_AJProperties)
    {
        if (name == *ajProperty->GetName())
        {
            retval = false;
            break;
        }
    }

    return retval;
}
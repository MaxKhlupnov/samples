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
#include "AllJoynHelper.h"
#include "DeviceMain.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation;

using namespace BridgeRT;
using namespace std;
using namespace DsbCommon;

AllJoynHelper::AllJoynHelper()
{
}

void AllJoynHelper::BuildBusObjectName(_In_ Platform::String ^inString, _Inout_ std::string &builtName)
{
    // only use a-z A-Z 0-9 char 
    // translate ' ' into '_'
    for (size_t index = 0; index < inString->Length(); index++)
    {
        wchar_t origChar = inString->Data()[index];
        if (::isalnum(origChar))
        {
            builtName += char(origChar);
        }
        else if (::isspace(origChar))
        {
            builtName += '_';
        }
    }
}

void AllJoynHelper::BuildPropertyOrMethodOrSignalName(_In_ Platform::String ^inString, _Inout_ std::string &builtName)
{
    // only use a-z A-Z 0-9 and _ char 
    // translate ' ' into '_'
    for (size_t index = 0; index < inString->Length(); index++)
    {
        wchar_t origChar = inString->Data()[index];
        if (::isalnum(origChar) ||
            '_' == char(origChar))
        {
            builtName += char(origChar);
        }
        else if (::isspace(origChar))
        {
            builtName += '_';
        }
    }
}

void AllJoynHelper::EncodeStringForServiceName(_In_ Platform::String ^inString, _Out_ std::string &encodeString)
{
    string tempString;

    encodeString.clear();

    // only use alpha numeric char
    for (size_t index = 0; index < inString->Length(); index++)
    {
        wchar_t origChar = inString->Data()[index];
        if (::isalnum(origChar))
        {
            tempString += char(origChar);
        }
    }

    if (0 != tempString.length())
    {
        // add '_' at the beginning if encoded value start with a number
        if (::isdigit(tempString[0]))
        {
            encodeString += '_';
        }

        encodeString += tempString;
    }
}

QStatus AllJoynHelper::SetMsgArg(_In_ IAdapterValue ^adapterValue, _Inout_ alljoyn_msgarg msgArg)
{
    QStatus status = ER_OK;
    IPropertyValue ^propertyValue = dynamic_cast<IPropertyValue ^>(adapterValue->Data);
    if (nullptr == propertyValue)
    {
        status = ER_BUS_BAD_VALUE;
        goto leave;
    }

    switch (propertyValue->Type)
    {
    case PropertyType::Boolean:
        status = alljoyn_msgarg_set(msgArg, "b", propertyValue->GetBoolean());
        break;

    case PropertyType::UInt8:
        status = alljoyn_msgarg_set(msgArg, "y", propertyValue->GetUInt8());
        break;

    case PropertyType::Char16:	__fallthrough;
    case PropertyType::Int16:
        status = alljoyn_msgarg_set(msgArg, "n", propertyValue->GetInt16());
        break;

    case PropertyType::UInt16:
        status = alljoyn_msgarg_set(msgArg, "q", propertyValue->GetUInt16());
        break;

    case PropertyType::Int32:
        status = alljoyn_msgarg_set(msgArg, "i", propertyValue->GetInt32());
        break;

    case PropertyType::UInt32:
        status = alljoyn_msgarg_set(msgArg, "u", propertyValue->GetUInt32());
        break;

    case PropertyType::Int64:
        status = alljoyn_msgarg_set(msgArg, "x", propertyValue->GetInt64());
        break;

    case PropertyType::UInt64:
        status = alljoyn_msgarg_set(msgArg, "t", propertyValue->GetUInt64());
        break;

    case PropertyType::Double:
        status = alljoyn_msgarg_set(msgArg, "d", propertyValue->GetDouble());
        break;

    case PropertyType::String:
    {
        string tempString = ConvertTo<string>(propertyValue->GetString());
        if (0 != tempString.length())
        {
            status = alljoyn_msgarg_set_and_stabilize(msgArg, "s", tempString.c_str());
        }
        else
        {
            // set empty string 
            status = alljoyn_msgarg_set(msgArg, "s", "");
        }
        break;
    }
    case PropertyType::StringArray:
    {
        Array<String^>^ strArray;
        propertyValue->GetStringArray(&strArray);
        if (strArray && strArray->Length > 0)
        {
            size_t i = 0;
            char** tempBuffer = new (nothrow) char*[strArray->Length];
            for (auto str : strArray)
            {                
                tempBuffer[i] = new char[str->Length() + 1] { 0 };
                strcpy_s(tempBuffer[i], str->Length() + 1, ConvertTo<string>(str).c_str());
                ++i;
            }
            status = alljoyn_msgarg_set_and_stabilize(msgArg, "as", strArray->Length, tempBuffer);

            //now delete temp buffer
            for (i = 0; i < strArray->Length; ++i)
            {
                delete[] tempBuffer[i];
            }
            delete[] tempBuffer;

        }
        else
        {
            // set empty string 
            status = alljoyn_msgarg_set(msgArg, "as", 1, "");
        }
        break;
    }
    default:
        status = ER_NOT_IMPLEMENTED;
        break;
    }

leave:
    return status;
}

QStatus AllJoynHelper::GetAdapterValue(_Inout_ IAdapterValue ^adapterValue, _In_ alljoyn_msgarg msgArg)
{
    QStatus status = ER_OK;
    IPropertyValue^ propertyValue = nullptr;

    // sanity check
    if (nullptr == adapterValue || nullptr == (propertyValue = dynamic_cast<IPropertyValue^>(adapterValue->Data)))
    {
        status = ER_BAD_ARG_1;
        goto leave;
    }

    switch (propertyValue->Type)
    {
    case PropertyType::Boolean:
    {
        bool tempVal;
        status = alljoyn_msgarg_get(msgArg, "b", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateBoolean(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::UInt8:
    {
        int8 tempVal;
        status = alljoyn_msgarg_get(msgArg, "y", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateUInt8(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::Char16:	__fallthrough;
    case PropertyType::Int16:
    {
        int16 tempVal;
        status = alljoyn_msgarg_get(msgArg, "n", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateInt16(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::UInt16:
    {
        uint16 tempVal;
        status = alljoyn_msgarg_get(msgArg, "q", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateUInt16(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::Int32:
    {
        int32 tempVal;
        status = alljoyn_msgarg_get(msgArg, "i", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateInt32(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::UInt32:
    {
        uint32 tempVal;
        status = alljoyn_msgarg_get(msgArg, "u", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateUInt32(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::Int64:
    {
        int64 tempVal;
        status = alljoyn_msgarg_get(msgArg, "x", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateInt64(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::UInt64:
    {
        uint64 tempVal;
        status = alljoyn_msgarg_get(msgArg, "t", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateUInt64(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::Double:
    {
        double tempVal;
        status = alljoyn_msgarg_get(msgArg, "d", &tempVal);
        if (ER_OK == status)
        {
            Platform::Object ^object = PropertyValue::CreateDouble(tempVal);
            adapterValue->Data = object;
        }
        break;
    }

    case PropertyType::String:
    {
        char *tempChar = nullptr;
        status = alljoyn_msgarg_get(msgArg, "s", &tempChar);
        if (ER_OK == status)
        {
            // convert char to wide char, create Platform::String
            // and update value
            adapterValue->Data = ref new String(ConvertTo<wstring>(string(tempChar)).c_str());;
        }
        break;
    }

    case PropertyType::StringArray:
    {
        alljoyn_msgarg entries;;
        size_t numVals = 0;

        status = alljoyn_msgarg_get(msgArg, "as", &numVals, &entries);
        if (ER_OK == status)
        {
            Platform::Array<String^>^ stringArray = ref new Platform::Array<String^>(numVals);
            
            for (size_t i = 0; i < numVals; ++i)
            {
                char *tempBuffer = nullptr;
                status = alljoyn_msgarg_get(alljoyn_msgarg_array_element(entries, i), "s", &tempBuffer);
                if (ER_OK == status)
                {
                    stringArray[i] = ref new String(ConvertTo<wstring>(string(tempBuffer)).c_str());
                }
                
            }
            adapterValue->Data = PropertyValue::CreateStringArray(stringArray);
        }
        break;
    }

    default:
        status = ER_NOT_IMPLEMENTED;
        break;
    }

leave:
    return status;
}

QStatus BridgeRT::AllJoynHelper::GetAdapterObject(IAdapterValue ^adapterValue, alljoyn_msgarg msgArg, DeviceMain *deviceMain)
{
    QStatus status = ER_OK;
    char *tempChar = nullptr;
    IAdapterProperty ^propertyParam = nullptr;

    if (TypeCode::Object != Type::GetTypeCode(adapterValue->Data->GetType()))
    {
        // not an object => nothing to do
        // (this is not an error for this routine)
        goto leave;
    }

    // IAdapterProperty is the only 
    // translate bus object path into IAdapterProperty
    status = alljoyn_msgarg_get(msgArg, "s", &tempChar);
    if (ER_OK != status)
    {
        // wrong signature
        status = ER_BUS_BAD_SIGNATURE;
        goto leave;
    }

    // find adapter object from string
    propertyParam = deviceMain->GetAdapterProperty(tempChar);
    if (nullptr == propertyParam)
    {
        // there is no matching IAdapterProperty for that argument
        status = ER_BUS_BAD_VALUE;
        goto leave;
    }
    adapterValue->Data = propertyParam;

leave:
    return status;
}

QStatus AllJoynHelper::SetMsgArgFromAdapterObject(_In_ IAdapterValue ^adapterValue, _Inout_ alljoyn_msgarg msgArg, _In_ DeviceMain *deviceMain)
{
    QStatus status = ER_OK;
    IAdapterProperty ^adapterProperty = dynamic_cast<IAdapterProperty ^>(adapterValue->Data);
    string busObjectPath;

    if (nullptr == adapterProperty)
    {
        // adapter Value doesn't contain a IAdapterProperty
        status = ER_BUS_BAD_VALUE;
        goto leave;
    }

    // get bus object path from IAdapterProperty and set message arg
    busObjectPath = deviceMain->GetBusObjectPath(adapterProperty);
    if (0 == busObjectPath.length())
    {
        status = alljoyn_msgarg_set(msgArg, "s", "");
    }
    else
    {
        // note that std::string must be copied in a temp buffer otherwise
        // alljoyn_msgarg_set will not be able to set the argument using c_str() method of std::string
        char *tempBuffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, busObjectPath.length() + 1);
        if (nullptr == tempBuffer)
        {
            status = ER_OUT_OF_MEMORY;
            goto leave;
        }
        CopyMemory(tempBuffer, busObjectPath.c_str(), busObjectPath.length());
        status = alljoyn_msgarg_set(msgArg, "s", tempBuffer);
        HeapFree(GetProcessHeap(), 0, tempBuffer);
    }

leave:
    return status;
}

QStatus AllJoynHelper::GetSignature(_In_ PropertyType propertyType, _Out_ std::string &signature)
{
    QStatus status = ER_OK;

    switch (propertyType)
    {
    case PropertyType::Boolean:
        signature = "b";
        break;

    case PropertyType::UInt8:
        signature = "y";
        break;

    case PropertyType::Char16:	__fallthrough;
    case PropertyType::Int16:
        signature = "n";
        break;

    case PropertyType::UInt16:
        signature = "q";
        break;

    case PropertyType::Int32:
        signature = "i";
        break;

    case PropertyType::UInt32:
        signature = "u";
        break;

    case PropertyType::Int64:
        signature = "x";
        break;

    case PropertyType::UInt64:
        signature = "t";
        break;

    case PropertyType::Double:
        signature = "d";
        break;

    case PropertyType::String:
        signature = "s";
        break;

    case PropertyType::StringArray:
        signature = "as";
        break;
        
    default:
        status = ER_NOT_IMPLEMENTED;
        break;
    }

    return status;
}

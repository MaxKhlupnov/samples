//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//

#pragma once

#include <alljoyn_c/Session.h>

static const char* DSB_DEVICENOTIFICATION_SIGNAL = "DeviceNotificationSignal";
static const char* DSB_SENDMSGTODEVICE_METHOD = "SendMessageToDeviceSynchronous";

static const char* DSB_SERVICE_NAME = "com.makhlu.DeviceSystemBridge.App1";
static const alljoyn_sessionport DSB_SERVICE_PORT = 27;

static const std::string DEFAULT_ROOT_SERVICE_NAME = "com.makhlu";
static const std::string BRIDGE_DEVICE_NAME = "device";

// {e2ddf0f1-b8c6-472c-bf15-6a6e7c983fac}
static const uint8_t DSB_APP_GUID[] = { 0xe2, 0xdd, 0xf0, 0xf1, 0xb8, 0xc6, 0x47, 0x2c, 0xbf, 0x15, 0x6a, 0x6e, 0x7c, 0x98, 0x3f, 0xac };
static const char* DSB_APP_NAME = "DSBServiceApp1";

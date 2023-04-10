// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "Interfaces/CameraInterface.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include <android_native_app_glue.h>
#include "EngineMinimal.h"
#include "Core.h"

class FCameraPlatform : public ICameraInterface
{
public:
	FCameraPlatform();
	virtual ~FCameraPlatform();

	virtual bool CheckInternetConnection() override;
	virtual bool CheckGooglePlayServices() override;
	virtual FString GetPersistentUniqueDeviceId() override;
	virtual FString GetDeviceId() override;
	virtual FString GetLanIp() override;

	// JNI Methods
	static jmethodID CheckInternetConnectionMethod;
	static jmethodID CheckGooglePlayServicesMethod;
	static jmethodID GetPersistentUniqueDeviceIdMethod;
	static jmethodID GetDeviceIdMethod;
	static jmethodID GetLanIpMethod;
};

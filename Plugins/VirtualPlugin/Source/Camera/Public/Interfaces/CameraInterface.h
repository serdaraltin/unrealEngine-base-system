// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "EngineMinimal.h"
#include "Core.h"

class ICameraInterface
{
public:
	/**
	* Check if internet connection is available on a device
	*
	* @return - true if connection is available
	*/
	virtual bool CheckInternetConnection() = 0;

	/**
	* Check if Google Play Services are available on a device
	*
	* @return - true if Google Play Services are available
	*/
	virtual bool CheckGooglePlayServices() = 0;


	/**
	* Return persistent Unique Device ID without reset after app reinstall
	*
	* @return - Unique Device ID
	*/
	virtual FString GetPersistentUniqueDeviceId();

	/**
	* Return Device ID. Should be unique but not guaranteed.
	*
	* @return - Device ID
	*/
	virtual FString GetDeviceId();

	/**
	* Return Device Lan Ip.
	*
	* @return - Lan Ip
	*/
	virtual FString GetLanIp();
};

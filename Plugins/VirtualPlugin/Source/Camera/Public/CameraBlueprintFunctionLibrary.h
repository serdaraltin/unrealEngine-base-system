// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CameraBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA_API UCameraBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	* Check if internet connection is available on a device
	*
	* @return - true if connection is available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CameraMobile)
		static bool CheckInternetConnection();

	/**
	* Check if Google Play Services are available on a device
	*
	* @return - true if Google Play Services are available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CameraMobile)
		static bool CheckGooglePlayServices();

	/**
	* Return persistent Unique Device ID without reset after app reinstall
	*
	* @return - Unique Device ID
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CameraMobile)
		static FString GetPersistentUniqueDeviceId();

	/**
	* Return Device ID. Should be unique but not guaranteed.
	*
	* @return - Unique Device ID
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CameraMobile)
		static FString GetDeviceId();

	/**
	* Return platform specific Auth Token retrieved from Online Subsystem
	*
	* @return - Auth Token
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CameraMobile)
		static FString GetAuthToken();

	/**
	* Return Device Lan Ip
	*
	* @return - Lan Ip
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CameraMobile)
		static FString GetLanIp();
	
};

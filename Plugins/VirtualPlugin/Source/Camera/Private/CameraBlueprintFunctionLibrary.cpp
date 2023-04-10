// Copyright 2020 ByOwls. All Rights Reserved.

#include "CameraBlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"

#include "SocketSubsystem.h"
#include "IPAddress.h"

#include "Interfaces/OnlineIdentityInterface.h"

UCameraBlueprintFunctionLibrary::UCameraBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UCameraBlueprintFunctionLibrary::CheckInternetConnection()
{
#if PLATFORM_ANDROID
	return FCameraModule::Get().GetPlatformInterface()->CheckInternetConnection();
#else
	return true;
#endif
}

bool UCameraBlueprintFunctionLibrary::CheckGooglePlayServices()
{
#if PLATFORM_ANDROID
	return FCameraModule::Get().GetPlatformInterface()->CheckGooglePlayServices();
#else
	return false;
#endif
}

FString UCameraBlueprintFunctionLibrary::GetPersistentUniqueDeviceId()
{
#if PLATFORM_ANDROID
	return FCameraModule::Get().GetPlatformInterface()->GetPersistentUniqueDeviceId();
#else
	return UKismetSystemLibrary::GetUniqueDeviceId();
#endif
}

FString UCameraBlueprintFunctionLibrary::GetDeviceId()
{
#if PLATFORM_ANDROID
	return FCameraModule::Get().GetPlatformInterface()->GetDeviceId();
#else
	return UKismetSystemLibrary::GetUniqueDeviceId();
#endif
}

FString UCameraBlueprintFunctionLibrary::GetAuthToken()
{

	auto OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
		if (OnlineIdentity.IsValid())
		{
			return OnlineIdentity->GetAuthToken(0);
		}
	}
	return FString();
}

FString UCameraBlueprintFunctionLibrary::GetLanIp()
{
#if PLATFORM_ANDROID
	return FCameraModule::Get().GetPlatformInterface()->GetLanIp();
#else
	return "windows";
#endif
}
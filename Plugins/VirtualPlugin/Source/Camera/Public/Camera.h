// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Core.h"
#include "Interfaces/CameraInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMobileUtils, Log, All);

typedef TSharedPtr<ICameraInterface, ESPMode::ThreadSafe> FCameraPlatformPtr;

class FCameraModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FCameraModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FCameraModule >("Camera");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("Camera");
	}

	inline FCameraPlatformPtr GetPlatformInterface() const
	{
		return PlatformInterface;
	}

protected:
	FCameraPlatformPtr PlatformInterface;
};
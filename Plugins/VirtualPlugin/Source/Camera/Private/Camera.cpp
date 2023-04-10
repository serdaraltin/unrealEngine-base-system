// Copyright 2020 ByOwls. All Rights Reserved.
#include "Camera.h"
#include "CameraBlueprintFunctionLibrary.h"

DEFINE_LOG_CATEGORY(LogMobileUtils);

#define LOCTEXT_NAMESPACE "FCameraModule"

void FCameraModule::StartupModule()
{
#if PLATFORM_ANDROID
	PlatformInterface = MakeShareable(new FCameraPlatform());
#endif
}

void FCameraModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCameraModule, Camera)
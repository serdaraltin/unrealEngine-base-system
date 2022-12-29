/**
* Copyright (C) 2017-2022 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "AwsDynamoDBTypes.h"

class AWSDYNAMODB_API FAwsDynamoDBModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
public:
	static bool LoadDependency(const FString& path, const FString& fileName, void*& handle);
	static void FreeDependency(void*& handle);
public:
	static void* s_CoreLibs;
	static void* s_CommonLibs;
	static void* s_StreamLibs;
	static void* s_ChecksumLibs;
	static void* s_DynamoDBLibs;
};

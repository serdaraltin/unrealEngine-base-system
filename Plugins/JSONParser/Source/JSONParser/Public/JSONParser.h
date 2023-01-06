// Copyright 2017-2021 David Romanski(Socke). All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/UserDefinedStruct.h"
#include "Dom/JsonObject.h"
#include "UObject/TextProperty.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/ObjectWriter.h"
#include "Serialization/ObjectReader.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Misc/Compression.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/FileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "JSONParser.generated.h"


class UJSONParserAsyncStringToObject;
class UJSONParserAsyncObjectToString;


UENUM(BlueprintType)
enum class EJsonCompressFormat : uint8
{
	E_Zlib 	UMETA(DisplayName = "Zlib"),
	E_Gzip	UMETA(DisplayName = "Gzip"),
	E_LZ4	UMETA(DisplayName = "LZ4")

};

UENUM(BlueprintType)
enum class EJsonDataType : uint8
{
	E_String 	UMETA(DisplayName = "String"),
	E_Number	UMETA(DisplayName = "Number"),
	E_Bool		UMETA(DisplayName = "Bool"),
	E_Object 	UMETA(DisplayName = "Object")

};

UENUM(BlueprintType)
enum class EJSONDirectoryType : uint8
{
	E_gd	UMETA(DisplayName = "Game directory"),
	E_ad 	UMETA(DisplayName = "Absolute directory")
};

UENUM(BlueprintType)
enum class EJSONJsonStructMapSetting : uint8
{
	E_object	UMETA(DisplayName = "Json Object (key must be a string)"),
	E_array 	UMETA(DisplayName = "Json Array ([0]=key,[1]=value)")
};


enum class EJSONThreadJobType : uint8
{
	E_None					UMETA(DisplayName = "none"),
	E_StringToObject		UMETA(DisplayName = "StringToObject"),
	E_FileToObject			UMETA(DisplayName = "FileToObject"),
	E_ObjectToString		UMETA(DisplayName = "ObjectToString"),
	E_ObjectToFile			UMETA(DisplayName = "ObjectToFile")
};



USTRUCT(BlueprintType)
struct FSJsonObject
{
	GENERATED_USTRUCT_BODY()

		TSharedPtr<FJsonObject>			 ue4Object;
	TArray< TSharedPtr<FJsonValue> > ue4Array;

	TArray<FSJsonObject>	asObjectArray;
	TArray<bool>			asBoolArray;
	TArray<float>			asNumberArray;
	TArray<FString>			asStringArray;

	FString jsonPBefore;
	FString jsonPAfter;
};


USTRUCT(BlueprintType)
struct FSFillStructWithJsonContainer
{
	GENERATED_USTRUCT_BODY()

		FProperty* structProperty = nullptr;
	void* structValue = nullptr;
};


struct FSJsonThreadJobData
{
	EJSONThreadJobType jobType = EJSONThreadJobType::E_None;
	UJSONParserAsyncStringToObject* JSONParserAsyncStringToObject = nullptr;
	UJSONParserAsyncObjectToString* JSONParserAsyncObjectToString = nullptr;
	EJSONDirectoryType directoryType = EJSONDirectoryType::E_ad;
	FString filePath = FString();
	struct FSJsonObject jsonObject;
	FString jsonString = FString();
	bool removeAndSavePadding = false;
	bool removeLineBreaks = false;
	bool addPadding = false;
};

#ifndef __JSONParserThread
#define __JSONParserThread
#include "JSONParserThread.h"
#endif

#ifndef __JSONParserAsyncStringToObject
#define __JSONParserAsyncStringToObject
#include "JSONParserAsyncStringToObject.h"
#endif

#ifndef __JSONParserAsyncObjectToString
#define __JSONParserAsyncObjectToString
#include "JSONParserAsyncObjectToString.h"
#endif

#ifndef __JSONParserBPLibrary
#define __JSONParserBPLibrary
#include "JSONParserBPLibrary.h"
#endif

class FJSONParserModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

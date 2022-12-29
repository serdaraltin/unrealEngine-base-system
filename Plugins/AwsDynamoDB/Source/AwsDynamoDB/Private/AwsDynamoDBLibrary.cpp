/**
* Copyright (C) 2017-2022 eelDev AB
*/

#include "AwsDynamoDBLibrary.h"
#include "AwsDynamoDBLogging.h"
#include "AwsDynamoDBPrivatePCH.h"
#include "Async/Async.h"

UDynamoDBClientObject* UAwsCoreDynamoDBPlatformObject::GetDynamoDBObject()
{
	return s_DynamoDBObjects.Num() > 0 ? s_DynamoDBObjects.Last() : nullptr;
}

TArray<UDynamoDBClientObject*> UAwsCoreDynamoDBPlatformObject::GetDynamoDBObjects()
{
	return s_DynamoDBObjects;
}

void UDynamoDBClientObject::Activate()
{
	LogVerbose("");
#if !DISABLE_DYNAMODB
	FString CertificatePath;

#if PLATFORM_ANDROID
	if (m_ClientConfiguration.CaFile.Len() == 0)
	{
		extern FString GFilePathBase;
		CertificatePath = GFilePathBase + FString("/UE4Game/") + FApp::GetProjectName() + "/" + FApp::GetProjectName() + FString("/Plugins/AwsDynamoDB/Source/ThirdParty");
		m_ClientConfiguration.CaFile = TCHAR_TO_UTF8(*FString::Printf(TEXT("%s/cacert.pem"), *CertificatePath));
	}
#endif

#if PLATFORM_LINUX || PLATFORM_WINDOWS || PLATFORM_IOS
	if (m_ClientConfiguration.CaFile.Len() == 0)
	{
		FString FileName = FString("AwsDynamoDB/Source/ThirdParty/cacert.pem");

#if PLATFORM_IOS
		FileName = FileName.ToLower();
#endif
		CertificatePath = FString::Printf(TEXT("%s%s"), *IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectPluginsDir()), *FileName);

		m_ClientConfiguration.CaFile = CertificatePath;
	}
#endif

	LogVerbose("Using Certificate: %s", *m_ClientConfiguration.CaFile);

	auto Task = [this]()
	{
		LogVerbose("Creating AwsCoreDynamoDbClient...");
		Aws::Client::ClientConfiguration* ClientConfiguration = new Aws::Client::ClientConfiguration(m_ClientConfiguration);
		ClientConfiguration->executor = Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>("DynamoDB", 10);
		Aws::Auth::AWSCredentials* Credentials = new Aws::Auth::AWSCredentials(m_Credentials);

		m_AwsCoreDynamoDBClient = MakeShareable(new Aws::DynamoDB::DynamoDBClient(*Credentials, *ClientConfiguration));
		m_Callback.ExecuteIfBound(this);
		LogVerbose("AwsCoreDynamoDbClient Created!");
	};

	auto Future = Async(EAsyncExecution::Thread, MoveTemp(Task));
#endif
}

UDynamoDBClientObject* UDynamoDBClientObject::CreateDynamoDbObject(FAwsDynamoDBCredentials Credentials, FAwsDynamoDBClientConfiguration ClientConfiguration, const FOnCloudTrailCallback& Callback)
{
	LogVerbose("");

	UDynamoDBClientObject* Proxy = NewObject<UDynamoDBClientObject>();
	Proxy->AddToRoot();
	Proxy->m_Credentials = Credentials;
	Proxy->m_ClientConfiguration = ClientConfiguration;
	Proxy->m_Callback = Callback;

	s_DynamoDBObjects.Add(Proxy);

	Proxy->Activate();

	return Proxy;
}

void UDynamoDBClientObject::DestroyDynamoDbObject()
{
	LogVerbose("Object (%s) marked for deletion", *this->GetName());

	RemoveFromRoot();
}

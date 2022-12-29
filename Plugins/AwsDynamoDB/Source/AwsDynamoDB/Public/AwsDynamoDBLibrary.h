/**
* Copyright (C) 2017-2022 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "AwsDynamoDBTypes.h"
#include "AwsDynamoDBLibrary.generated.h"

static TArray<class UDynamoDBClientObject*> s_DynamoDBObjects;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCloudTrailCallback, UDynamoDBClientObject*, dynamoDBClientObject);

UCLASS(BlueprintType)
class AWSDYNAMODB_API UDynamoDBClientObject : public UObject
{
	GENERATED_BODY()
public:
	virtual ~UDynamoDBClientObject()
	{
#if !DISABLE_DYNAMODB
		m_AwsCoreDynamoDBClient.Reset();
		m_AwsCoreDynamoDBClient = nullptr;
#endif
	}

protected:
#if !DISABLE_DYNAMODB
	TSharedPtr<Aws::DynamoDB::DynamoDBClient, ESPMode::ThreadSafe> m_AwsCoreDynamoDBClient;
#endif
	FAwsDynamoDBCredentials m_Credentials;
	FAwsDynamoDBClientConfiguration m_ClientConfiguration;
	FOnCloudTrailCallback m_Callback;
public:
	void Activate();
#if !DISABLE_DYNAMODB
	Aws::DynamoDB::DynamoDBClient* GetDynamoDBClient() { return m_AwsCoreDynamoDBClient.Get(); }
	Aws::DynamoDB::DynamoDBClient* GetDynamoDBClient() const { return m_AwsCoreDynamoDBClient.Get(); }
#endif
public:
	/**
	* Create a new DynamoDB Client Object
	* @param: callback will be executed when the object has been created
	**/
	UFUNCTION(BlueprintCallable, Category = "AwsCore|DynamoDB", meta = (AutoCreateRefTerm = "callback"))
	static UDynamoDBClientObject* CreateDynamoDbObject(FAwsDynamoDBCredentials Credentials, FAwsDynamoDBClientConfiguration ClientConfiguration, const FOnCloudTrailCallback& Callback);
	UFUNCTION(BlueprintCallable, Category = "AwsCore|DynamoDB")
	void DestroyDynamoDbObject();
};

UCLASS()
class AWSDYNAMODB_API UAwsCoreDynamoDBPlatformObject : public UObject
{
	GENERATED_BODY()
public:
	/*
	* Utility function to get the last created game lift object
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AwsCore|DynamoDB")
	static UDynamoDBClientObject* GetDynamoDBObject();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AwsCore|DynamoDB")
	static TArray<UDynamoDBClientObject*> GetDynamoDBObjects();
};

UCLASS()
class AWSDYNAMODB_API UDynamoDBAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	virtual void Activate() override { Super::Activate(); }
	virtual void SetReadyToDestroy() override
	{
		Super::SetReadyToDestroy();
		RemoveFromRoot();
	}
};

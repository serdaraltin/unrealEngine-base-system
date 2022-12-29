/**
* Copyright (C) 2017-2022 eelDev AB
*/

#include "AwsDynamoDBAsync.h"
#include "AwsDynamoDBLogging.h"
#include "AwsDynamoDBPrivatePCH.h"

UDynamoBatchGetItem* UDynamoBatchGetItem::BatchGetItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FBatchGetItemRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoBatchGetItem>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoBatchGetItem::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::BatchGetItemRequest Request = {};

	for (auto& Element : m_RequestData.RequestItems)
	{
		Aws::DynamoDB::Model::KeysAndAttributes Attribute;

		for (auto& Keys : Element.Value.Keys)
		{
			Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Map;

			for (auto& MapElement : Keys.Map)
			{
				Aws::String String = TCHAR_TO_UTF8(*MapElement.Key);
				Aws::DynamoDB::Model::AttributeValue AttributeValue = MapElement.Value;

				Map.insert({String, AttributeValue});
			}

			Attribute.AddKeys(Map);
		}

		for (auto& AttributesToGet : Element.Value.AttributesToGet)
		{
			Attribute.AddAttributesToGet(TCHAR_TO_UTF8(*AttributesToGet));
		}

		Attribute.SetConsistentRead(Element.Value.bConsistentRead);

		if (Element.Value.ProjectionExpression.IsEmpty() == false)
			Attribute.SetProjectionExpression(TCHAR_TO_UTF8(*Element.Value.ProjectionExpression));

		for (auto& ExpressionAttributeNames : Element.Value.ExpressionAttributeNames)
		{
			Attribute.AddExpressionAttributeNames(TCHAR_TO_UTF8(*ExpressionAttributeNames.Key), TCHAR_TO_UTF8(*ExpressionAttributeNames.Value));
		}

		Request.AddRequestItems(TCHAR_TO_UTF8(*Element.Key), Attribute);
	}

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::BatchGetItemRequest&, Aws::DynamoDB::Model::BatchGetItemOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->BatchGetItemAsync(Request, CallbackLambda);
#endif
}

UDynamoBatchWriteItem* UDynamoBatchWriteItem::BatchWriteItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FBatchWriteItemRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoBatchWriteItem>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoBatchWriteItem::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::BatchWriteItemRequest Request = {};

	for (auto& Element : m_RequestData.RequestItems)
	{
		if (Element.Key.Len() == 0) continue;

		Aws::Vector<Aws::DynamoDB::Model::WriteRequest> DataArray;

		for (auto& WriteRequest : Element.Value.Requests)
		{
			DataArray.emplace_back(WriteRequest);
		}

		Request.AddRequestItems(TCHAR_TO_UTF8(*Element.Key), DataArray);
	}

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NONE)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.ReturnItemCollectionMetrics != EReturnItemCollectionMetrics::NOT_SET)
		Request.SetReturnItemCollectionMetrics(static_cast<Aws::DynamoDB::Model::ReturnItemCollectionMetrics>(m_RequestData.ReturnItemCollectionMetrics));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::BatchWriteItemRequest&, Aws::DynamoDB::Model::BatchWriteItemOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->BatchWriteItemAsync(Request, CallbackLambda);
#endif
}

UDynamoCreateBackup* UDynamoCreateBackup::CreateBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FCreateBackupRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoCreateBackup>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoCreateBackup::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::CreateBackupRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.BackupName.IsEmpty() == false)
		Request.SetBackupName(TCHAR_TO_UTF8(*m_RequestData.BackupName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::CreateBackupRequest&, Aws::DynamoDB::Model::CreateBackupOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->CreateBackupAsync(Request, CallbackLambda);
#endif
}

UDynamoCreateGlobalTable* UDynamoCreateGlobalTable::CreateGlobalTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FCreateGlobalTableRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoCreateGlobalTable>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoCreateGlobalTable::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::CreateGlobalTableRequest Request = {};

	if (m_RequestData.GlobalTableName.IsEmpty() == false)
		Request.SetGlobalTableName(TCHAR_TO_UTF8(*m_RequestData.GlobalTableName));

	for (auto& Element : m_RequestData.ReplicationGroup)
		Request.AddReplicationGroup(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::CreateGlobalTableRequest&, Aws::DynamoDB::Model::CreateGlobalTableOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->CreateGlobalTableAsync(Request, CallbackLambda);
#endif
}

UDynamoCreateTable* UDynamoCreateTable::CreateTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FCreateTableRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoCreateTable>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoCreateTable::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::CreateTableRequest Request = {};

	for (auto& Element : m_RequestData.AttributeDefinitions)
		Request.AddAttributeDefinitions(Element);

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	for (auto& Element : m_RequestData.KeySchema)
		Request.AddKeySchema(Element);

	for (auto& Element : m_RequestData.LocalSecondaryIndexes)
		Request.AddLocalSecondaryIndexes(Element);

	for (auto& Element : m_RequestData.GlobalSecondaryIndexes)
		Request.AddGlobalSecondaryIndexes(Element);

	if (m_RequestData.BillingMode != EBillingMode::NOT_SET)
		Request.SetBillingMode(static_cast<Aws::DynamoDB::Model::BillingMode>(m_RequestData.BillingMode));

	Request.SetProvisionedThroughput(m_RequestData.ProvisionedThroughput);
	Request.SetStreamSpecification(m_RequestData.StreamSpecification);
	Request.SetSSESpecification(m_RequestData.SSESpecification);

	for (auto& Element : m_RequestData.Tags)
		Request.AddTags(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::CreateTableRequest&, Aws::DynamoDB::Model::CreateTableOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->CreateTableAsync(Request, CallbackLambda);
#endif
}

UDynamoDeleteBackup* UDynamoDeleteBackup::DeleteBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDeleteBackupRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDeleteBackup>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDeleteBackup::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DeleteBackupRequest Request = {};

	if (m_RequestData.BackupArn.IsEmpty() == false)
		Request.SetBackupArn(TCHAR_TO_UTF8(*m_RequestData.BackupArn));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DeleteBackupRequest&, Aws::DynamoDB::Model::DeleteBackupOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DeleteBackupAsync(Request, CallbackLambda);
#endif
}

UDynamoDeleteItem* UDynamoDeleteItem::DeleteItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDeleteItemRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDeleteItem>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDeleteItem::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DeleteItemRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	for (auto& Element : m_RequestData.Key)
		Request.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	for (auto& Element : m_RequestData.Expected)
		Request.AddExpected(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	if (m_RequestData.ConditionalOperator != EConditionalOperator::NOT_SET)
		Request.SetConditionalOperator(static_cast<Aws::DynamoDB::Model::ConditionalOperator>(m_RequestData.ConditionalOperator));

	if (m_RequestData.ReturnValues != EReturnValue::NOT_SET)
		Request.SetReturnValues(static_cast<Aws::DynamoDB::Model::ReturnValue>(m_RequestData.ReturnValues));

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.ReturnItemCollectionMetrics != EReturnItemCollectionMetrics::NOT_SET)
		Request.SetReturnItemCollectionMetrics(static_cast<Aws::DynamoDB::Model::ReturnItemCollectionMetrics>(m_RequestData.ReturnItemCollectionMetrics));

	if (m_RequestData.ConditionExpression.IsEmpty() == false)
		Request.SetConditionExpression(TCHAR_TO_UTF8(*m_RequestData.ConditionExpression));

	for (auto& Element : m_RequestData.ExpressionAttributeNames)
		Request.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	for (auto& Element : m_RequestData.ExpressionAttributeValues)
		Request.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DeleteItemRequest&, Aws::DynamoDB::Model::DeleteItemOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DeleteItemAsync(Request, CallbackLambda);
#endif
}

UDynamoDeleteTable* UDynamoDeleteTable::DeleteTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDeleteTableRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDeleteTable>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDeleteTable::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DeleteTableRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DeleteTableRequest&, Aws::DynamoDB::Model::DeleteTableOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DeleteTableAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeBackup* UDynamoDescribeBackup::DescribeBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeBackupRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeBackup>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeBackup::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeBackupRequest Request = {};

	if (m_RequestData.BackupArn.IsEmpty() == false)
		Request.SetBackupArn(TCHAR_TO_UTF8(*m_RequestData.BackupArn));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeBackupRequest&, Aws::DynamoDB::Model::DescribeBackupOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeBackupAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeContinuousBackups* UDynamoDescribeContinuousBackups::DescribeContinuousBackups(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeContinuousBackupsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeContinuousBackups>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeContinuousBackups::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeContinuousBackupsRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeContinuousBackupsRequest&, Aws::DynamoDB::Model::DescribeContinuousBackupsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeContinuousBackupsAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeContributorInsights* UDynamoDescribeContributorInsights::DescribeContributorInsights(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeContributorInsightsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeContributorInsights>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeContributorInsights::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeContributorInsightsRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.IndexName.IsEmpty() == false)
		Request.SetIndexName(TCHAR_TO_UTF8(*m_RequestData.IndexName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeContributorInsightsRequest&, Aws::DynamoDB::Model::DescribeContributorInsightsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeContributorInsightsAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeEndpoints* UDynamoDescribeEndpoints::DescribeEndpoints(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeEndpointsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeEndpoints>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeEndpoints::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeEndpointsRequest Request = {};

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeEndpointsRequest&, Aws::DynamoDB::Model::DescribeEndpointsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeEndpointsAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeGlobalTable* UDynamoDescribeGlobalTable::DescribeGlobalTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeGlobalTableRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeGlobalTable>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeGlobalTable::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeGlobalTableRequest Request = {};

	if (m_RequestData.GlobalTableName.IsEmpty() == false)
		Request.SetGlobalTableName(TCHAR_TO_UTF8(*m_RequestData.GlobalTableName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeGlobalTableRequest&, Aws::DynamoDB::Model::DescribeGlobalTableOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeGlobalTableAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeGlobalTableSettings* UDynamoDescribeGlobalTableSettings::DescribeGlobalTableSettings(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeGlobalTableSettingsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeGlobalTableSettings>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeGlobalTableSettings::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeGlobalTableSettingsRequest Request = {};

	if (m_RequestData.GlobalTableName.IsEmpty() == false)
		Request.SetGlobalTableName(TCHAR_TO_UTF8(*m_RequestData.GlobalTableName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeGlobalTableSettingsRequest&, Aws::DynamoDB::Model::DescribeGlobalTableSettingsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeGlobalTableSettingsAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeLimits* UDynamoDescribeLimits::DescribeLimits(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeLimitsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeLimits>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeLimits::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeLimitsRequest Request = {};

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeLimitsRequest&, Aws::DynamoDB::Model::DescribeLimitsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeLimitsAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeTable* UDynamoDescribeTable::DescribeTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeTableRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeTable>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeTable::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeTableRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeTableRequest&, Aws::DynamoDB::Model::DescribeTableOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeTableAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeTableReplicaAutoScaling* UDynamoDescribeTableReplicaAutoScaling::DescribeTableReplicaAutoScaling(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeTableReplicaAutoScalingRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeTableReplicaAutoScaling>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeTableReplicaAutoScaling::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeTableReplicaAutoScalingRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeTableReplicaAutoScalingRequest&, Aws::DynamoDB::Model::DescribeTableReplicaAutoScalingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeTableReplicaAutoScalingAsync(Request, CallbackLambda);
#endif
}

UDynamoDescribeTimeToLive* UDynamoDescribeTimeToLive::DescribeTimeToLive(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeTimeToLiveRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoDescribeTimeToLive>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoDescribeTimeToLive::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::DescribeTimeToLiveRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::DescribeTimeToLiveRequest&, Aws::DynamoDB::Model::DescribeTimeToLiveOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->DescribeTimeToLiveAsync(Request, CallbackLambda);
#endif
}

UDynamoGetItem* UDynamoGetItem::GetItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FGetItemRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoGetItem>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoGetItem::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::GetItemRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	for (auto& Element : m_RequestData.Key)
		Request.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	for (auto& Element : m_RequestData.AttributesToGet)
		Request.AddAttributesToGet(TCHAR_TO_UTF8(*Element));

	Request.SetConsistentRead(m_RequestData.bConsistentRead);

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.ProjectionExpression.IsEmpty() == false)
		Request.SetProjectionExpression(TCHAR_TO_UTF8(*m_RequestData.ProjectionExpression));

	for (auto& Element : m_RequestData.ExpressionAttributeNames)
		Request.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::GetItemRequest&, Aws::DynamoDB::Model::GetItemOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->GetItemAsync(Request, CallbackLambda);
#endif
}

UDynamoListBackups* UDynamoListBackups::ListBackups(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListBackupsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoListBackups>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoListBackups::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::ListBackupsRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.Limit > 0)
		Request.SetLimit(m_RequestData.Limit);

	if (m_RequestData.TimeRangeLowerBound > 0)
		Request.SetTimeRangeLowerBound(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.TimeRangeLowerBound.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.TimeRangeUpperBound > 0)
		Request.SetTimeRangeUpperBound(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.TimeRangeUpperBound.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.ExclusiveStartBackupArn.IsEmpty() == false)
		Request.SetExclusiveStartBackupArn(TCHAR_TO_UTF8(*m_RequestData.ExclusiveStartBackupArn));

	if (m_RequestData.BackupType != EBackupTypeFilter::NOT_SET)
		Request.SetBackupType(static_cast<Aws::DynamoDB::Model::BackupTypeFilter>(m_RequestData.BackupType));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::ListBackupsRequest&, Aws::DynamoDB::Model::ListBackupsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->ListBackupsAsync(Request, CallbackLambda);
#endif
}

UDynamoListContributorInsights* UDynamoListContributorInsights::ListContributorInsights(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListContributorInsightsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoListContributorInsights>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoListContributorInsights::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::ListContributorInsightsRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.NextToken.IsEmpty() == false)
		Request.SetNextToken(TCHAR_TO_UTF8(*m_RequestData.NextToken));

	if (m_RequestData.MaxResults > 0)
		Request.SetMaxResults(m_RequestData.MaxResults);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::ListContributorInsightsRequest&, Aws::DynamoDB::Model::ListContributorInsightsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->ListContributorInsightsAsync(Request, CallbackLambda);
#endif
}

UDynamoListGlobalTables* UDynamoListGlobalTables::ListGlobalTables(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListGlobalTablesRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoListGlobalTables>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoListGlobalTables::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::ListGlobalTablesRequest Request = {};

	if (m_RequestData.ExclusiveStartGlobalTableName.IsEmpty() == false)
		Request.SetExclusiveStartGlobalTableName(TCHAR_TO_UTF8(*m_RequestData.ExclusiveStartGlobalTableName));

	if (m_RequestData.Limit > 0)
		Request.SetLimit(m_RequestData.Limit);

	if (m_RequestData.RegionName.IsEmpty() == false)
		Request.SetRegionName(TCHAR_TO_UTF8(*m_RequestData.RegionName));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::ListGlobalTablesRequest&, Aws::DynamoDB::Model::ListGlobalTablesOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->ListGlobalTablesAsync(Request, CallbackLambda);
#endif
}

UDynamoListTables* UDynamoListTables::ListTables(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListTablesRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoListTables>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoListTables::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::ListTablesRequest Request = {};

	if (m_RequestData.ExclusiveStartTableName.IsEmpty() == false)
		Request.SetExclusiveStartTableName(TCHAR_TO_UTF8(*m_RequestData.ExclusiveStartTableName));

	if (m_RequestData.Limit > 0)
		Request.SetLimit(m_RequestData.Limit);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::ListTablesRequest&, Aws::DynamoDB::Model::ListTablesOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->ListTablesAsync(Request, CallbackLambda);
#endif
}

UDynamoListTagsOfResource* UDynamoListTagsOfResource::ListTagsOfResource(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListTagsOfResourceRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoListTagsOfResource>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoListTagsOfResource::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::ListTagsOfResourceRequest Request = {};

	if (m_RequestData.ResourceArn.IsEmpty() == false)
		Request.SetResourceArn(TCHAR_TO_UTF8(*m_RequestData.ResourceArn));

	if (m_RequestData.NextToken.IsEmpty() == false)
		Request.SetNextToken(TCHAR_TO_UTF8(*m_RequestData.NextToken));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::ListTagsOfResourceRequest&, Aws::DynamoDB::Model::ListTagsOfResourceOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->ListTagsOfResourceAsync(Request, CallbackLambda);
#endif
}

UDynamoPutItem* UDynamoPutItem::PutItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FPutItemRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoPutItem>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoPutItem::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::PutItemRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	for (auto& Element : m_RequestData.Item)
		Request.AddItem(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	for (auto& Element : m_RequestData.Expected)
		Request.AddExpected(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	if (m_RequestData.ReturnValues != EReturnValue::NOT_SET)
		Request.SetReturnValues(static_cast<Aws::DynamoDB::Model::ReturnValue>(m_RequestData.ReturnValues));

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.ReturnItemCollectionMetrics != EReturnItemCollectionMetrics::NOT_SET)
		Request.SetReturnItemCollectionMetrics(static_cast<Aws::DynamoDB::Model::ReturnItemCollectionMetrics>(m_RequestData.ReturnItemCollectionMetrics));

	if (m_RequestData.ConditionalOperator != EConditionalOperator::NOT_SET)
		Request.SetConditionalOperator(static_cast<Aws::DynamoDB::Model::ConditionalOperator>(m_RequestData.ConditionalOperator));

	if (m_RequestData.ConditionExpression.IsEmpty() == false)
		Request.SetConditionExpression(TCHAR_TO_UTF8(*m_RequestData.ConditionExpression));

	for (auto& Element : m_RequestData.ExpressionAttributeNames)
		Request.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	for (auto& Element : m_RequestData.ExpressionAttributeValues)
		Request.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::PutItemRequest&, Aws::DynamoDB::Model::PutItemOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->PutItemAsync(Request, CallbackLambda);
#endif
}

UDynamoQuery* UDynamoQuery::Query(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FQueryRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoQuery>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoQuery::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::QueryRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.IndexName.IsEmpty() == false)
		Request.SetIndexName(TCHAR_TO_UTF8(*m_RequestData.IndexName));

	if (m_RequestData.Select != ESelect::NOT_SET)
		Request.SetSelect(static_cast<Aws::DynamoDB::Model::Select>(m_RequestData.Select));

	for (auto& Element : m_RequestData.AttributesToGet)
		Request.AddAttributesToGet(TCHAR_TO_UTF8(*Element));

	if (m_RequestData.Limit > 0)
		Request.SetLimit(m_RequestData.Limit);

	Request.SetConsistentRead(m_RequestData.bConsistentRead);

	for (auto& Element : m_RequestData.KeyConditions)
		Request.AddKeyConditions(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	for (auto& Element : m_RequestData.QueryFilter)
		Request.AddQueryFilter(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	if (m_RequestData.ConditionalOperator != EConditionalOperator::NOT_SET)
		Request.SetConditionalOperator(static_cast<Aws::DynamoDB::Model::ConditionalOperator>(m_RequestData.ConditionalOperator));

	Request.SetScanIndexForward(m_RequestData.bScanIndexForward);

	for (auto& Element : m_RequestData.ExclusiveStartKey)
		Request.AddExclusiveStartKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.ProjectionExpression.IsEmpty() == false)
		Request.SetProjectionExpression(TCHAR_TO_UTF8(*m_RequestData.ProjectionExpression));

	if (m_RequestData.FilterExpression.IsEmpty() == false)
		Request.SetFilterExpression(TCHAR_TO_UTF8(*m_RequestData.FilterExpression));

	if (m_RequestData.KeyConditionExpression.IsEmpty() == false)
		Request.SetKeyConditionExpression(TCHAR_TO_UTF8(*m_RequestData.KeyConditionExpression));

	for (auto& Element : m_RequestData.ExpressionAttributeNames)
		Request.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	for (auto& Element : m_RequestData.ExpressionAttributeValues)
		Request.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::QueryRequest&, Aws::DynamoDB::Model::QueryOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->QueryAsync(Request, CallbackLambda);
#endif
}

UDynamoRestoreTableFromBackup* UDynamoRestoreTableFromBackup::RestoreTableFromBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FRestoreTableFromBackupRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoRestoreTableFromBackup>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoRestoreTableFromBackup::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::RestoreTableFromBackupRequest Request = {};

	if (m_RequestData.TargetTableName.IsEmpty() == false)
		Request.SetTargetTableName(TCHAR_TO_UTF8(*m_RequestData.TargetTableName));

	if (m_RequestData.BackupArn.IsEmpty() == false)
		Request.SetBackupArn(TCHAR_TO_UTF8(*m_RequestData.BackupArn));

	if (m_RequestData.BillingModeOverride != EBillingMode::NOT_SET)
		Request.SetBillingModeOverride(static_cast<Aws::DynamoDB::Model::BillingMode>(m_RequestData.BillingModeOverride));

	for (auto& Element : m_RequestData.GlobalSecondaryIndexOverride)
		Request.AddGlobalSecondaryIndexOverride(Element);

	for (auto& Element : m_RequestData.LocalSecondaryIndexOverride)
		Request.AddLocalSecondaryIndexOverride(Element);

	Request.SetProvisionedThroughputOverride(m_RequestData.ProvisionedThroughputOverride);
	Request.SetSSESpecificationOverride(m_RequestData.SSESpecificationOverride);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::RestoreTableFromBackupRequest&, Aws::DynamoDB::Model::RestoreTableFromBackupOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->RestoreTableFromBackupAsync(Request, CallbackLambda);
#endif
}

UDynamoRestoreTableToPointInTime* UDynamoRestoreTableToPointInTime::RestoreTableToPointInTime(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FRestoreTableToPointInTimeRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoRestoreTableToPointInTime>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoRestoreTableToPointInTime::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::RestoreTableToPointInTimeRequest Request = {};

	if (m_RequestData.SourceTableArn.IsEmpty() == false)
		Request.SetSourceTableArn(TCHAR_TO_UTF8(*m_RequestData.SourceTableArn));

	if (m_RequestData.SourceTableName.IsEmpty() == false)
		Request.SetSourceTableName(TCHAR_TO_UTF8(*m_RequestData.SourceTableName));

	if (m_RequestData.TargetTableName.IsEmpty() == false)
		Request.SetTargetTableName(TCHAR_TO_UTF8(*m_RequestData.TargetTableName));

	Request.SetUseLatestRestorableTime(m_RequestData.bUseLatestRestorableTime);

	if (m_RequestData.RestoreDateTime > 0)
		Request.SetRestoreDateTime(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.RestoreDateTime.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.BillingModeOverride != EBillingMode::NOT_SET)
		Request.SetBillingModeOverride(static_cast<Aws::DynamoDB::Model::BillingMode>(m_RequestData.BillingModeOverride));

	for (auto& Element : m_RequestData.GlobalSecondaryIndexOverride)
		Request.AddGlobalSecondaryIndexOverride(Element);

	for (auto& Element : m_RequestData.LocalSecondaryIndexOverride)
		Request.AddLocalSecondaryIndexOverride(Element);

	Request.SetProvisionedThroughputOverride(m_RequestData.ProvisionedThroughputOverride);
	Request.SetSSESpecificationOverride(m_RequestData.SSESpecificationOverride);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::RestoreTableToPointInTimeRequest&, Aws::DynamoDB::Model::RestoreTableToPointInTimeOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->RestoreTableToPointInTimeAsync(Request, CallbackLambda);
#endif
}

UDynamoScan* UDynamoScan::Scan(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FScanRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoScan>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoScan::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::ScanRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.IndexName.IsEmpty() == false)
		Request.SetIndexName(TCHAR_TO_UTF8(*m_RequestData.IndexName));

	for (auto& Element : m_RequestData.AttributesToGet)
		Request.AddAttributesToGet(TCHAR_TO_UTF8(*Element));

	if (m_RequestData.Limit > 0)
		Request.SetLimit(m_RequestData.Limit);

	if (m_RequestData.Select != ESelect::NOT_SET)
		Request.SetSelect(static_cast<Aws::DynamoDB::Model::Select>(m_RequestData.Select));

	for (auto& Element : m_RequestData.ScanFilter)
		Request.AddScanFilter(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	if (m_RequestData.ConditionalOperator != EConditionalOperator::NOT_SET)
		Request.SetConditionalOperator(static_cast<Aws::DynamoDB::Model::ConditionalOperator>(m_RequestData.ConditionalOperator));

	for (auto& Element : m_RequestData.ExclusiveStartKey)
		Request.AddExclusiveStartKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.TotalSegments > 0)
		Request.SetTotalSegments(m_RequestData.TotalSegments);

	if (m_RequestData.Segment > 0)
		Request.SetSegment(m_RequestData.Segment);

	if (m_RequestData.ProjectionExpression.IsEmpty() == false)
		Request.SetProjectionExpression(TCHAR_TO_UTF8(*m_RequestData.ProjectionExpression));

	if (m_RequestData.FilterExpression.IsEmpty() == false)
		Request.SetFilterExpression(TCHAR_TO_UTF8(*m_RequestData.FilterExpression));

	for (auto& Element : m_RequestData.ExpressionAttributeNames)
	{
		char KeyAnsi[256];
		char ValueAnsi[2048];

		FCStringAnsi::Strncpy(KeyAnsi, TCHAR_TO_UTF8(*Element.Key), Element.Key.GetAllocatedSize());
		FCStringAnsi::Strncpy(ValueAnsi, TCHAR_TO_UTF8(*Element.Value), Element.Value.GetAllocatedSize());

		Request.AddExpressionAttributeNames(KeyAnsi, ValueAnsi);
	}

	for (auto& Element : m_RequestData.ExpressionAttributeValues)
		Request.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	Request.SetConsistentRead(m_RequestData.bConsistentRead);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::ScanRequest&, Aws::DynamoDB::Model::ScanOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->ScanAsync(Request, CallbackLambda);
#endif
}

UDynamoTagResource* UDynamoTagResource::TagResource(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FTagResourceRequestDynamoDB& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoTagResource>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoTagResource::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::TagResourceRequest Request = {};

	if (m_RequestData.ResourceArn.IsEmpty() == false)
		Request.SetResourceArn(TCHAR_TO_UTF8(*m_RequestData.ResourceArn));

	for (auto& Element : m_RequestData.Tags)
		Request.AddTags(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::TagResourceRequest&, Aws::DynamoDB::Model::TagResourceOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->TagResourceAsync(Request, CallbackLambda);
#endif
}

UDynamoTransactGetItems* UDynamoTransactGetItems::TransactGetItems(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FTransactGetItemsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoTransactGetItems>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoTransactGetItems::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::TransactGetItemsRequest Request = {};

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	for (auto& Element : m_RequestData.TransactItems)
		Request.AddTransactItems(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::TransactGetItemsRequest&, Aws::DynamoDB::Model::TransactGetItemsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->TransactGetItemsAsync(Request, CallbackLambda);
#endif
}

UDynamoTransactWriteItems* UDynamoTransactWriteItems::TransactWriteItems(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FTransactWriteItemsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoTransactWriteItems>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoTransactWriteItems::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::TransactWriteItemsRequest Request = {};

	for (auto& Element : m_RequestData.TransactItems)
		Request.AddTransactItems(Element);

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.ReturnItemCollectionMetrics != EReturnItemCollectionMetrics::NOT_SET)
		Request.SetReturnItemCollectionMetrics(static_cast<Aws::DynamoDB::Model::ReturnItemCollectionMetrics>(m_RequestData.ReturnItemCollectionMetrics));

	if (m_RequestData.ClientRequestToken.IsEmpty() == false)
		Request.SetClientRequestToken(TCHAR_TO_UTF8(*m_RequestData.ClientRequestToken));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::TransactWriteItemsRequest&, Aws::DynamoDB::Model::TransactWriteItemsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->TransactWriteItemsAsync(Request, CallbackLambda);
#endif
}

UDynamoUntagResource* UDynamoUntagResource::UntagResource(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUntagResourceRequestDynamoDB& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUntagResource>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUntagResource::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UntagResourceRequest Request = {};

	if (m_RequestData.ResourceArn.IsEmpty() == false)
		Request.SetResourceArn(TCHAR_TO_UTF8(*m_RequestData.ResourceArn));

	for (auto& Element : m_RequestData.TagKeys)
		Request.AddTagKeys(TCHAR_TO_UTF8(*Element));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UntagResourceRequest&, Aws::DynamoDB::Model::UntagResourceOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UntagResourceAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateContinuousBackups* UDynamoUpdateContinuousBackups::UpdateContinuousBackups(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateContinuousBackupsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateContinuousBackups>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateContinuousBackups::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateContinuousBackupsRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	Request.SetPointInTimeRecoverySpecification(m_RequestData.PointInTimeRecoverySpecification);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateContinuousBackupsRequest&, Aws::DynamoDB::Model::UpdateContinuousBackupsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateContinuousBackupsAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateContributorInsights* UDynamoUpdateContributorInsights::UpdateContributorInsights(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateContributorInsightsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateContributorInsights>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateContributorInsights::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateContributorInsightsRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.IndexName.IsEmpty() == false)
		Request.SetIndexName(TCHAR_TO_UTF8(*m_RequestData.IndexName));

	if (m_RequestData.ContributorInsightsAction != EContributorInsightsAction::NOT_SET)
		Request.SetContributorInsightsAction(static_cast<Aws::DynamoDB::Model::ContributorInsightsAction>(m_RequestData.ContributorInsightsAction));

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateContributorInsightsRequest&, Aws::DynamoDB::Model::UpdateContributorInsightsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateContributorInsightsAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateGlobalTable* UDynamoUpdateGlobalTable::UpdateGlobalTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateGlobalTableRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateGlobalTable>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateGlobalTable::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateGlobalTableRequest Request = {};

	if (m_RequestData.GlobalTableName.IsEmpty() == false)
		Request.SetGlobalTableName(TCHAR_TO_UTF8(*m_RequestData.GlobalTableName));

	for (auto& Element : m_RequestData.ReplicaUpdates)
		Request.AddReplicaUpdates(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateGlobalTableRequest&, Aws::DynamoDB::Model::UpdateGlobalTableOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateGlobalTableAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateGlobalTableSettings* UDynamoUpdateGlobalTableSettings::UpdateGlobalTableSettings(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateGlobalTableSettingsRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateGlobalTableSettings>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateGlobalTableSettings::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateGlobalTableSettingsRequest Request = {};

	if (m_RequestData.GlobalTableName.IsEmpty() == false)
		Request.SetGlobalTableName(TCHAR_TO_UTF8(*m_RequestData.GlobalTableName));

	if (m_RequestData.GlobalTableBillingMode != EBillingMode::NOT_SET)
		Request.SetGlobalTableBillingMode(static_cast<Aws::DynamoDB::Model::BillingMode>(m_RequestData.GlobalTableBillingMode));

	if (m_RequestData.GlobalTableProvisionedWriteCapacityUnits.IsEmpty() == false)
		Request.SetGlobalTableProvisionedWriteCapacityUnits(FCString::Atod(*m_RequestData.GlobalTableProvisionedWriteCapacityUnits));

	Request.SetGlobalTableProvisionedWriteCapacityAutoScalingSettingsUpdate(m_RequestData.GlobalTableProvisionedWriteCapacityAutoScalingSettingsUpdate);

	for (auto& Element : m_RequestData.GlobalTableGlobalSecondaryIndexSettingsUpdate)
		Request.AddGlobalTableGlobalSecondaryIndexSettingsUpdate(Element);

	for (auto& Element : m_RequestData.ReplicaSettingsUpdate)
		Request.AddReplicaSettingsUpdate(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateGlobalTableSettingsRequest&, Aws::DynamoDB::Model::UpdateGlobalTableSettingsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateGlobalTableSettingsAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateItem* UDynamoUpdateItem::UpdateItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateItemRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateItem>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateItem::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateItemRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	for (auto& Element : m_RequestData.Key)
		Request.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	for (auto& Element : m_RequestData.AttributeUpdates)
		Request.AddAttributeUpdates(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	for (auto& Element : m_RequestData.Expected)
		Request.AddExpected(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	if (m_RequestData.ConditionalOperator != EConditionalOperator::NOT_SET)
		Request.SetConditionalOperator(static_cast<Aws::DynamoDB::Model::ConditionalOperator>(m_RequestData.ConditionalOperator));

	if (m_RequestData.ReturnValues != EReturnValue::NOT_SET)
		Request.SetReturnValues(static_cast<Aws::DynamoDB::Model::ReturnValue>(m_RequestData.ReturnValues));

	if (m_RequestData.ReturnConsumedCapacity != EReturnConsumedCapacity::NOT_SET)
		Request.SetReturnConsumedCapacity(static_cast<Aws::DynamoDB::Model::ReturnConsumedCapacity>(m_RequestData.ReturnConsumedCapacity));

	if (m_RequestData.ReturnItemCollectionMetrics != EReturnItemCollectionMetrics::NOT_SET)
		Request.SetReturnItemCollectionMetrics(static_cast<Aws::DynamoDB::Model::ReturnItemCollectionMetrics>(m_RequestData.ReturnItemCollectionMetrics));

	if (m_RequestData.UpdateExpression.IsEmpty() == false)
		Request.SetUpdateExpression(TCHAR_TO_UTF8(*m_RequestData.UpdateExpression));

	if (m_RequestData.ConditionExpression.IsEmpty() == false)
		Request.SetConditionExpression(TCHAR_TO_UTF8(*m_RequestData.ConditionExpression));

	for (auto& Element : m_RequestData.ExpressionAttributeNames)
		Request.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	for (auto& Element : m_RequestData.ExpressionAttributeValues)
		Request.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateItemRequest&, Aws::DynamoDB::Model::UpdateItemOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateItemAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateTable* UDynamoUpdateTable::UpdateTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateTableRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateTable>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateTable::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateTableRequest Request = {};

	for (auto& Element : m_RequestData.AttributeDefinitions)
		Request.AddAttributeDefinitions(Element);

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	if (m_RequestData.BillingMode != EBillingMode::NOT_SET)
		Request.SetBillingMode(static_cast<Aws::DynamoDB::Model::BillingMode>(m_RequestData.BillingMode));

	Request.SetProvisionedThroughput(m_RequestData.ProvisionedThroughput);

	for (auto& Element : m_RequestData.GlobalSecondaryIndexUpdates)
		Request.AddGlobalSecondaryIndexUpdates(Element);

	Request.SetStreamSpecification(m_RequestData.StreamSpecification);
	Request.SetSSESpecification(m_RequestData.SSESpecification);

	for (auto& Element : m_RequestData.ReplicaUpdates)
		Request.AddReplicaUpdates(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateTableRequest&, Aws::DynamoDB::Model::UpdateTableOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateTableAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateTableReplicaAutoScaling* UDynamoUpdateTableReplicaAutoScaling::UpdateTableReplicaAutoScaling(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateTableReplicaAutoScalingRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateTableReplicaAutoScaling>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateTableReplicaAutoScaling::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateTableReplicaAutoScalingRequest Request = {};

	for (auto& Element : m_RequestData.GlobalSecondaryIndexUpdates)
		Request.AddGlobalSecondaryIndexUpdates(Element);

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	Request.SetProvisionedWriteCapacityAutoScalingUpdate(m_RequestData.ProvisionedWriteCapacityAutoScalingUpdate);

	for (auto& Element : m_RequestData.ReplicaUpdates)
		Request.AddReplicaUpdates(Element);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateTableReplicaAutoScalingRequest&, Aws::DynamoDB::Model::UpdateTableReplicaAutoScalingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateTableReplicaAutoScalingAsync(Request, CallbackLambda);
#endif
}

UDynamoUpdateTimeToLive* UDynamoUpdateTimeToLive::UpdateTimeToLive(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateTimeToLiveRequest& request)
{
	LogVerbose("");

	auto Platform = NewObject<UDynamoUpdateTimeToLive>();
	Platform->AddToRoot();
	Platform->m_DynamoDBObject = dynamoDBObject;
	Platform->m_RequestData = request;

	return Platform;
}

void UDynamoUpdateTimeToLive::Activate()
{
#if !DISABLE_DYNAMODB
	LogVerbose("");
	check(m_DynamoDBObject);
	check(m_DynamoDBObject->GetDynamoDBClient());

	Aws::DynamoDB::Model::UpdateTimeToLiveRequest Request = {};

	if (m_RequestData.TableName.IsEmpty() == false)
		Request.SetTableName(TCHAR_TO_UTF8(*m_RequestData.TableName));

	Request.SetTimeToLiveSpecification(m_RequestData.TimeToLiveSpecification);

	auto CallbackLambda = [this](const Aws::DynamoDB::DynamoDBClient*, const Aws::DynamoDB::Model::UpdateTimeToLiveRequest&, Aws::DynamoDB::Model::UpdateTimeToLiveOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<EDynamoDBErrors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_DynamoDBObject->GetDynamoDBClient()->UpdateTimeToLiveAsync(Request, CallbackLambda);
#endif
}

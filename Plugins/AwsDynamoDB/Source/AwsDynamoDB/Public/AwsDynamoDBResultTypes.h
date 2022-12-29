/**
* Copyright (C) 2017-2022 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "AwsDynamoDBTypes.h"
#include "AwsDynamoDBResultTypes.generated.h"

UENUM(BlueprintType)
enum class EContributorInsightsStatus : uint8
{
	NOT_SET,
	ENABLING,
	ENABLED,
	DISABLING,
	DISABLED,
	FAILED
};

UENUM(BlueprintType)
enum class EPointInTimeRecoveryStatus : uint8
{
	NOT_SET,
	ENABLED,
	DISABLED
};

UENUM(BlueprintType)
enum class EContinuousBackupsStatus : uint8
{
	NOT_SET,
	ENABLED,
	DISABLED
};

UENUM(BlueprintType)
enum class ETimeToLiveStatus : uint8
{
	NOT_SET,
	ENABLING,
	DISABLING,
	ENABLED,
	DISABLED
};

UENUM(BlueprintType)
enum class ESSEStatus : uint8
{
	NOT_SET,
	ENABLING,
	ENABLED,
	DISABLING,
	DISABLED,
	UPDATING
};

UENUM(BlueprintType)
enum class EIndexStatus : uint8
{
	NOT_SET,
	CREATING,
	UPDATING,
	DELETING,
	ACTIVE
};

UENUM(BlueprintType)
enum class EBackupStatus : uint8
{
	NOT_SET,
	CREATING,
	DELETED,
	AVAILABLE
};

UENUM(BlueprintType)
enum class EBackupType : uint8
{
	NOT_SET,
	USER,
	SYSTEM,
	AWS_BACKUP
};

UENUM(BlueprintType)
enum class EGlobalTableStatus : uint8
{
	NOT_SET,
	CREATING,
	ACTIVE,
	DELETING,
	UPDATING
};

UENUM(BlueprintType)
enum class EReplicaStatus : uint8
{
	NOT_SET,
	CREATING,
	CREATION_FAILED,
	UPDATING,
	DELETING,
	ACTIVE
};

UENUM(BlueprintType)
enum class ETableStatus : uint8
{
	NOT_SET,
	CREATING,
	UPDATING,
	DELETING,
	ACTIVE,
	INACCESSIBLE_ENCRYPTION_CREDENTIALS,
	ARCHIVING,
	ARCHIVED
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FCapacity
{
	GENERATED_BODY()
public:
	FCapacity() = default;
#if !DISABLE_DYNAMODB
	FCapacity(const Aws::DynamoDB::Model::Capacity& Data)
		: ReadCapacityUnits(LexToString(Data.GetReadCapacityUnits()))
		  , WriteCapacityUnits(LexToString(Data.GetWriteCapacityUnits()))
		  , CapacityUnits(LexToString(Data.GetCapacityUnits()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString WriteCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString CapacityUnits;
};

USTRUCT(BlueprintType)
struct FConsumedCapacity
{
	GENERATED_BODY()
public:
	FConsumedCapacity() = default;
#if !DISABLE_DYNAMODB
	FConsumedCapacity(const Aws::DynamoDB::Model::ConsumedCapacity& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , CapacityUnits(LexToString(Data.GetCapacityUnits()))
		  , ReadCapacityUnits(LexToString(Data.GetReadCapacityUnits()))
		  , WriteCapacityUnits(LexToString(Data.GetWriteCapacityUnits()))
		  , Table(Data.GetTable())
	{
		for (auto& Element : Data.GetLocalSecondaryIndexes())
			LocalSecondaryIndexes.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);

		for (auto& Element : Data.GetGlobalSecondaryIndexes())
			GlobalSecondaryIndexes.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString CapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString WriteCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FCapacity Table;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FCapacity> LocalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FCapacity> GlobalSecondaryIndexes;
};

USTRUCT(BlueprintType)
struct FItemCollectionMetrics
{
	GENERATED_BODY()
public:
	FItemCollectionMetrics() = default;
#if !DISABLE_DYNAMODB
	FItemCollectionMetrics(const Aws::DynamoDB::Model::ItemCollectionMetrics& Data)
	{
		for (auto& Element : Data.GetItemCollectionKey())
			ItemCollectionKey.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);

		for (auto& Element : Data.GetSizeEstimateRangeGB())
			SizeEstimateRangeGB.Add(LexToString(Element));
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ItemCollectionKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> SizeEstimateRangeGB;
};

USTRUCT(BlueprintType)
struct FResponseTest
{
	GENERATED_BODY()
public:
	FResponseTest() = default;

	FResponseTest(const FString& key, TArray<FAttributeValue> values)
		: Key(key)
		  , Metrics(values)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeValue> Metrics;
};

USTRUCT(BlueprintType)
struct FKeysMapResult
{
	GENERATED_BODY()
public:
	FKeysMapResult() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Map;
};

USTRUCT(BlueprintType)
struct FKeysArrayResult
{
	GENERATED_BODY()
public:
	FKeysArrayResult() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeysMapResult> Array;
};

USTRUCT(BlueprintType)
struct FBatchGetItemResult
{
	GENERATED_BODY()
public:
	FBatchGetItemResult() = default;
#if !DISABLE_DYNAMODB
	FBatchGetItemResult(const Aws::DynamoDB::Model::BatchGetItemResult& Data)
	{
		for (auto& Element : Data.GetUnprocessedKeys())
			UnprocessedKeys.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);

		for (auto& Element : Data.GetConsumedCapacity())
			ConsumedCapacity.Add(Element);

		for (auto& Element : Data.GetResponses())
		{
			FKeysArrayResult Array;

			for (auto MappedElement : Element.second)
			{
				FKeysMapResult MapResult;

				for (auto MappedElementResult : MappedElement)
				{
					MapResult.Map.Add(UTF8_TO_TCHAR(MappedElementResult.first.c_str()), MappedElementResult.second);
				}

				Array.Array.Add(MapResult);
			}

			Responses.Add(UTF8_TO_TCHAR(Element.first.c_str()), Array);
		}
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FKeysArrayResult> Responses;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FKeysAndAttributes> UnprocessedKeys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FConsumedCapacity> ConsumedCapacity;
};

USTRUCT(BlueprintType)
struct FItemCollectionMetricsTest
{
	GENERATED_BODY()
public:
	FItemCollectionMetricsTest() = default;

	FItemCollectionMetricsTest(const FString& key, TArray<FItemCollectionMetrics> values)
		: Key(key)
		  , Metrics(values)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FItemCollectionMetrics> Metrics;
};

USTRUCT(BlueprintType)
struct FWriteRequestsTest
{
	GENERATED_BODY()
public:
	FWriteRequestsTest() = default;

	FWriteRequestsTest(const FString& key, TArray<FWriteRequest> values)
		: Key(key)
		  , WriteRequests(values)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FWriteRequest> WriteRequests;
};

USTRUCT(BlueprintType)
struct FBatchWriteItemResult
{
	GENERATED_BODY()
public:
	FBatchWriteItemResult() = default;
#if !DISABLE_DYNAMODB
	FBatchWriteItemResult(const Aws::DynamoDB::Model::BatchWriteItemResult& Data)
	{
		for (auto& Element : Data.GetUnprocessedItems())
		{
			TArray<FWriteRequest> WriteRequests;

			for (auto& ElementRequest : Element.second)
				WriteRequests.Add(ElementRequest);

			UnprocessedItems.Add(FWriteRequestsTest(UTF8_TO_TCHAR(Element.first.c_str()), WriteRequests));
		}

		for (auto& Element : Data.GetItemCollectionMetrics())
		{
			TArray<FItemCollectionMetrics> Metrics;

			for (auto& Metric : Element.second)
				Metrics.Add(Metric);

			ItemCollectionMetrics.Add(FItemCollectionMetricsTest(UTF8_TO_TCHAR(Element.first.c_str()), Metrics));
		}

		for (auto& Element : Data.GetConsumedCapacity())
			ConsumedCapacity.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FWriteRequestsTest> UnprocessedItems;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FItemCollectionMetricsTest> ItemCollectionMetrics;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FConsumedCapacity> ConsumedCapacity;
};

USTRUCT(BlueprintType)
struct FBackupDetails
{
	GENERATED_BODY()
public:
	FBackupDetails()
		: BackupStatus(EBackupStatus::NOT_SET)
		  , BackupType(EBackupType::NOT_SET)
		  , BackupCreationDateTime(0)
		  , BackupExpiryDateTime(0)
	{
	}
#if !DISABLE_DYNAMODB
	FBackupDetails(const Aws::DynamoDB::Model::BackupDetails& Data)
		: BackupArn(UTF8_TO_TCHAR(Data.GetBackupArn().c_str()))
		  , BackupName(UTF8_TO_TCHAR(Data.GetBackupName().c_str()))
		  , BackupSizeBytes(LexToString(Data.GetBackupSizeBytes()))
		  , BackupStatus(static_cast<EBackupStatus>(Data.GetBackupStatus()))
		  , BackupType(static_cast<EBackupType>(Data.GetBackupType()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetBackupCreationDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), BackupCreationDateTime);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetBackupExpiryDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), BackupExpiryDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupSizeBytes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBackupStatus BackupStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBackupType BackupType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime BackupCreationDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime BackupExpiryDateTime;
};

USTRUCT(BlueprintType)
struct FCreateBackupResult
{
	GENERATED_BODY()
public:
	FCreateBackupResult() = default;
#if !DISABLE_DYNAMODB
	FCreateBackupResult(const Aws::DynamoDB::Model::CreateBackupResult& Data)
		: BackupDetails(Data.GetBackupDetails())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FBackupDetails BackupDetails;
};

USTRUCT(BlueprintType)
struct FReplicaGlobalSecondaryIndexDescription
{
	GENERATED_BODY()
public:
	FReplicaGlobalSecondaryIndexDescription() = default;
#if !DISABLE_DYNAMODB
	FReplicaGlobalSecondaryIndexDescription(const Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndexDescription& Data)
		: IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , ProvisionedThroughputOverride(Data.GetProvisionedThroughputOverride())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughputOverride ProvisionedThroughputOverride;
};

USTRUCT(BlueprintType)
struct FReplicaDescription
{
	GENERATED_BODY()
public:
	FReplicaDescription()
		: ReplicaStatus(EReplicaStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FReplicaDescription(const Aws::DynamoDB::Model::ReplicaDescription& Data)
		: RegionName(UTF8_TO_TCHAR(Data.GetRegionName().c_str()))
		  , ReplicaStatus(static_cast<EReplicaStatus>(Data.GetReplicaStatus()))
		  , ReplicaStatusDescription(UTF8_TO_TCHAR(Data.GetReplicaStatusDescription().c_str()))
		  , ReplicaStatusPercentProgress(UTF8_TO_TCHAR(Data.GetReplicaStatusPercentProgress().c_str()))
		  , KMSMasterKeyId(UTF8_TO_TCHAR(Data.GetKMSMasterKeyId().c_str()))
		  , ProvisionedThroughputOverride(Data.GetProvisionedThroughputOverride())
	{
		for (auto& Element : Data.GetGlobalSecondaryIndexes())
			GlobalSecondaryIndexes.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReplicaStatus ReplicaStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReplicaStatusDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReplicaStatusPercentProgress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString KMSMasterKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughputOverride ProvisionedThroughputOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaGlobalSecondaryIndexDescription> GlobalSecondaryIndexes;
};

USTRUCT(BlueprintType)
struct FGlobalTableDescription
{
	GENERATED_BODY()
public:
	FGlobalTableDescription()
		: CreationDateTime(0)
		  , GlobalTableStatus(EGlobalTableStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FGlobalTableDescription(const Aws::DynamoDB::Model::GlobalTableDescription& Data)
		: GlobalTableArn(UTF8_TO_TCHAR(Data.GetGlobalTableArn().c_str()))
		  , GlobalTableStatus(static_cast<EGlobalTableStatus>(Data.GetGlobalTableStatus()))
		  , GlobalTableName(UTF8_TO_TCHAR(Data.GetGlobalTableName().c_str()))
	{
		for (auto& Element : Data.GetReplicationGroup())
			ReplicationGroup.Add(Element);

		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetCreationDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), CreationDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaDescription> ReplicationGroup;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime CreationDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EGlobalTableStatus GlobalTableStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
};

USTRUCT(BlueprintType)
struct FCreateGlobalTableResult
{
	GENERATED_BODY()
public:
	FCreateGlobalTableResult() = default;
#if !DISABLE_DYNAMODB
	FCreateGlobalTableResult(const Aws::DynamoDB::Model::CreateGlobalTableResult& Data)
		: GlobalTableDescription(Data.GetGlobalTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FGlobalTableDescription GlobalTableDescription;
};

USTRUCT(BlueprintType)
struct FProvisionedThroughputDescription
{
	GENERATED_BODY()
public:
	FProvisionedThroughputDescription()
		: LastIncreaseDateTime(0)
		  , LastDecreaseDateTime(0)
	{
	}

#if !DISABLE_DYNAMODB
	FProvisionedThroughputDescription(const Aws::DynamoDB::Model::ProvisionedThroughputDescription& Data)
		: NumberOfDecreasesToday(LexToString(Data.GetNumberOfDecreasesToday()))
		  , ReadCapacityUnits(LexToString(Data.GetReadCapacityUnits()))
		  , WriteCapacityUnits(LexToString(Data.GetWriteCapacityUnits()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastIncreaseDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastIncreaseDateTime);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastDecreaseDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastDecreaseDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime LastIncreaseDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime LastDecreaseDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString NumberOfDecreasesToday;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString WriteCapacityUnits;
};

USTRUCT(BlueprintType)
struct FBillingModeSummary
{
	GENERATED_BODY()
public:
	FBillingModeSummary()
		: BillingMode(EBillingMode::NOT_SET)
		  , LastUpdateToPayPerRequestDateTime(0)
	{
	}
#if !DISABLE_DYNAMODB
	FBillingModeSummary(const Aws::DynamoDB::Model::BillingModeSummary& Data)
		: BillingMode(static_cast<EBillingMode>(Data.GetBillingMode()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastUpdateToPayPerRequestDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastUpdateToPayPerRequestDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBillingMode BillingMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime LastUpdateToPayPerRequestDateTime;
};

USTRUCT(BlueprintType)
struct FSSEDescription
{
	GENERATED_BODY()
public:
	FSSEDescription()
		: Status(ESSEStatus::NOT_SET)
		  , SSEType(ESSEType::NOT_SET)
		  , InaccessibleEncryptionDateTime(0)
	{
	}
#if !DISABLE_DYNAMODB
	FSSEDescription(const Aws::DynamoDB::Model::SSEDescription& Data)
		: Status(static_cast<ESSEStatus>(Data.GetStatus()))
		  , SSEType(static_cast<ESSEType>(Data.GetSSEType()))
		  , KMSMasterKeyArn(UTF8_TO_TCHAR(Data.GetKMSMasterKeyArn().c_str()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetInaccessibleEncryptionDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), InaccessibleEncryptionDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ESSEStatus Status;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ESSEType SSEType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString KMSMasterKeyArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime InaccessibleEncryptionDateTime;
};

USTRUCT(BlueprintType)
struct FArchivalSummary
{
	GENERATED_BODY()
public:
	FArchivalSummary()
		: ArchivalDateTime(0)
	{
	}

#if !DISABLE_DYNAMODB
	FArchivalSummary(const Aws::DynamoDB::Model::ArchivalSummary& Data)
		: ArchivalReason(UTF8_TO_TCHAR(Data.GetArchivalReason().c_str()))
		  , ArchivalBackupArn(UTF8_TO_TCHAR(Data.GetArchivalBackupArn().c_str()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetArchivalDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), ArchivalDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime ArchivalDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ArchivalReason;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ArchivalBackupArn;
};

USTRUCT(BlueprintType)
struct FRestoreSummary
{
	GENERATED_BODY()
public:
	FRestoreSummary()
		: RestoreDateTime(0)
		  , bRestoreInProgress(false)
	{
	}
#if !DISABLE_DYNAMODB
	FRestoreSummary(const Aws::DynamoDB::Model::RestoreSummary& Data)
		: SourceBackupArn(UTF8_TO_TCHAR(Data.GetSourceBackupArn().c_str()))
		  , SourceTableArn(UTF8_TO_TCHAR(Data.GetSourceTableArn().c_str()))
		  , bRestoreInProgress(Data.GetRestoreInProgress())
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetRestoreDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), RestoreDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString SourceBackupArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString SourceTableArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime RestoreDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bRestoreInProgress;
};

USTRUCT(BlueprintType)
struct FLocalSecondaryIndexDescription
{
	GENERATED_BODY()
public:
	FLocalSecondaryIndexDescription() = default;
#if !DISABLE_DYNAMODB
	FLocalSecondaryIndexDescription(const Aws::DynamoDB::Model::LocalSecondaryIndexDescription& Data)
		: IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , Projection(Data.GetProjection())
		  , IndexSizeBytes(LexToString(Data.GetIndexSizeBytes()))
		  , ItemCount(LexToString(Data.GetItemCount()))
		  , IndexArn(UTF8_TO_TCHAR(Data.GetIndexArn().c_str()))
	{
		for (auto& Element : Data.GetKeySchema())
			KeySchema.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeySchemaElement> KeySchema;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProjection Projection;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexSizeBytes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ItemCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexArn;
};

USTRUCT(BlueprintType)
struct FGlobalSecondaryIndexDescription
{
	GENERATED_BODY()
public:
	FGlobalSecondaryIndexDescription()
		: IndexStatus(EIndexStatus::NOT_SET)
		  , bBackfilling(false)
	{
	}
#if !DISABLE_DYNAMODB
	FGlobalSecondaryIndexDescription(const Aws::DynamoDB::Model::GlobalSecondaryIndexDescription& Data)
		: IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , Projection(Data.GetProjection())
		  , IndexStatus(static_cast<EIndexStatus>(Data.GetIndexStatus()))
		  , bBackfilling(Data.GetBackfilling())
		  , ProvisionedThroughput(Data.GetProvisionedThroughput())
		  , IndexSizeBytes(LexToString(Data.GetIndexSizeBytes()))
		  , ItemCount(LexToString(Data.GetItemCount()))
		  , IndexArn(UTF8_TO_TCHAR(Data.GetIndexArn().c_str()))
	{
		for (auto& Element : Data.GetKeySchema())
			KeySchema.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeySchemaElement> KeySchema;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProjection Projection;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EIndexStatus IndexStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bBackfilling;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughputDescription ProvisionedThroughput;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexSizeBytes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ItemCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexArn;
};

USTRUCT(BlueprintType)
struct FTableDescription
{
	GENERATED_BODY()
public:
	FTableDescription()
		: TableStatus(ETableStatus::NOT_SET)
		  , CreationDateTime(0)
	{
	}
#if !DISABLE_DYNAMODB
	FTableDescription(const Aws::DynamoDB::Model::TableDescription& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , TableStatus(static_cast<ETableStatus>(Data.GetTableStatus()))
		  , ProvisionedThroughput(Data.GetProvisionedThroughput())
		  , TableSizeBytes(LexToString(Data.GetTableSizeBytes()))
		  , ItemCount(LexToString(Data.GetItemCount()))
		  , TableArn(UTF8_TO_TCHAR(Data.GetTableArn().c_str()))
		  , TableId(UTF8_TO_TCHAR(Data.GetTableId().c_str()))
		  , BillingModeSummary(Data.GetBillingModeSummary())
		  , StreamSpecification(Data.GetStreamSpecification())
		  , LatestStreamLabel(UTF8_TO_TCHAR(Data.GetLatestStreamLabel().c_str()))
		  , LatestStreamArn(UTF8_TO_TCHAR(Data.GetLatestStreamArn().c_str()))
		  , GlobalTableVersion(UTF8_TO_TCHAR(Data.GetGlobalTableVersion().c_str()))
		  , RestoreSummary(Data.GetRestoreSummary())
		  , SSEDescription(Data.GetSSEDescription())
		  , ArchivalSummary(Data.GetArchivalSummary())
	{
		for (auto& Element : Data.GetAttributeDefinitions())
			AttributeDefinitions.Add(Element);

		for (auto& Element : Data.GetKeySchema())
			KeySchema.Add(Element);

		for (auto& Element : Data.GetLocalSecondaryIndexes())
			LocalSecondaryIndexes.Add(Element);

		for (auto& Element : Data.GetGlobalSecondaryIndexes())
			GlobalSecondaryIndexes.Add(Element);

		for (auto& Element : Data.GetReplicas())
			Replicas.Add(Element);

		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetCreationDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), CreationDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeDefinition> AttributeDefinitions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeySchemaElement> KeySchema;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ETableStatus TableStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime CreationDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughputDescription ProvisionedThroughput;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableSizeBytes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ItemCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FBillingModeSummary BillingModeSummary;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FLocalSecondaryIndexDescription> LocalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalSecondaryIndexDescription> GlobalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FStreamSpecification StreamSpecification;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString LatestStreamLabel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString LatestStreamArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableVersion;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaDescription> Replicas;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FRestoreSummary RestoreSummary;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSSEDescription SSEDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FArchivalSummary ArchivalSummary;
};

USTRUCT(BlueprintType)
struct FLocalSecondaryIndexInfo
{
	GENERATED_BODY()
public:
	FLocalSecondaryIndexInfo() = default;
#if !DISABLE_DYNAMODB
	FLocalSecondaryIndexInfo(const Aws::DynamoDB::Model::LocalSecondaryIndexInfo& Data)
		: IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , Projection(Data.GetProjection())
	{
		for (auto& Element : Data.GetKeySchema())
			KeySchema.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeySchemaElement> KeySchema;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProjection Projection;
};

USTRUCT(BlueprintType)
struct FTimeToLiveDescription
{
	GENERATED_BODY()
public:
	FTimeToLiveDescription()
		: TimeToLiveStatus(ETimeToLiveStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FTimeToLiveDescription(const Aws::DynamoDB::Model::TimeToLiveDescription& Data)
		: TimeToLiveStatus(static_cast<ETimeToLiveStatus>(Data.GetTimeToLiveStatus()))
		  , AttributeName(UTF8_TO_TCHAR(Data.GetAttributeName().c_str()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ETimeToLiveStatus TimeToLiveStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString AttributeName;
};

USTRUCT(BlueprintType)
struct FGlobalSecondaryIndexInfo
{
	GENERATED_BODY()
public:
	FGlobalSecondaryIndexInfo() = default;
#if !DISABLE_DYNAMODB
	FGlobalSecondaryIndexInfo(const Aws::DynamoDB::Model::GlobalSecondaryIndexInfo& Data)
		: IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , Projection(Data.GetProjection())
		  , ProvisionedThroughput(Data.GetProvisionedThroughput())
	{
		for (auto& Element : Data.GetKeySchema())
			KeySchema.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeySchemaElement> KeySchema;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProjection Projection;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughput ProvisionedThroughput;
};

USTRUCT(BlueprintType)
struct FSourceTableFeatureDetails
{
	GENERATED_BODY()
public:
	FSourceTableFeatureDetails() = default;
#if !DISABLE_DYNAMODB
	FSourceTableFeatureDetails(const Aws::DynamoDB::Model::SourceTableFeatureDetails& Data)
		: StreamDescription(Data.GetStreamDescription())
		  , TimeToLiveDescription(Data.GetTimeToLiveDescription())
		  , SSEDescription(Data.GetSSEDescription())
	{
		for (auto& Element : Data.GetLocalSecondaryIndexes())
			LocalSecondaryIndexes.Add(Element);

		for (auto& Element : Data.GetGlobalSecondaryIndexes())
			GlobalSecondaryIndexes.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FLocalSecondaryIndexInfo> LocalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalSecondaryIndexInfo> GlobalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FStreamSpecification StreamDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTimeToLiveDescription TimeToLiveDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSSEDescription SSEDescription;
};

USTRUCT(BlueprintType)
struct FSourceTableDetails
{
	GENERATED_BODY()
public:
	FSourceTableDetails()
		: TableCreationDateTime(0)
		  , BillingMode(EBillingMode::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FSourceTableDetails(const Aws::DynamoDB::Model::SourceTableDetails& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , TableId(UTF8_TO_TCHAR(Data.GetTableId().c_str()))
		  , TableArn(UTF8_TO_TCHAR(Data.GetTableArn().c_str()))
		  , TableSizeBytes(LexToString(Data.GetTableSizeBytes()))
		  , ProvisionedThroughput(Data.GetProvisionedThroughput())
		  , ItemCount(LexToString(Data.GetItemCount()))
		  , BillingMode(static_cast<EBillingMode>(Data.GetBillingMode()))
	{
		for (auto& Element : Data.GetKeySchema())
			KeySchema.Add(Element);

		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetTableCreationDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), TableCreationDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableSizeBytes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeySchemaElement> KeySchema;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime TableCreationDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughput ProvisionedThroughput;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ItemCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBillingMode BillingMode;
};

USTRUCT(BlueprintType)
struct FBackupDescription
{
	GENERATED_BODY()
public:
	FBackupDescription() = default;
#if !DISABLE_DYNAMODB
	FBackupDescription(const Aws::DynamoDB::Model::BackupDescription& Data)
		: BackupDetails(Data.GetBackupDetails())
		  , SourceTableDetails(Data.GetSourceTableDetails())
		  , SourceTableFeatureDetails(Data.GetSourceTableFeatureDetails())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FBackupDetails BackupDetails;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSourceTableDetails SourceTableDetails;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSourceTableFeatureDetails SourceTableFeatureDetails;
};

USTRUCT(BlueprintType)
struct FCreateTableResult
{
	GENERATED_BODY()
public:
	FCreateTableResult() = default;
#if !DISABLE_DYNAMODB
	FCreateTableResult(const Aws::DynamoDB::Model::CreateTableResult& Data)
		: TableDescription(Data.GetTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableDescription TableDescription;
};

USTRUCT(BlueprintType)
struct FDeleteBackupResult
{
	GENERATED_BODY()
public:
	FDeleteBackupResult() = default;
#if !DISABLE_DYNAMODB
	FDeleteBackupResult(const Aws::DynamoDB::Model::DeleteBackupResult& Data)
		: BackupDescription(Data.GetBackupDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FBackupDescription BackupDescription;
};

USTRUCT(BlueprintType)
struct FDeleteItemResult
{
	GENERATED_BODY()
public:
	FDeleteItemResult() = default;
#if !DISABLE_DYNAMODB
	FDeleteItemResult(const Aws::DynamoDB::Model::DeleteItemResult& Data)
		: ConsumedCapacity(Data.GetConsumedCapacity())
		  , ItemCollectionMetrics(Data.GetItemCollectionMetrics())
	{
		for (auto& Element : Data.GetAttributes())
			Attributes.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Attributes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FConsumedCapacity ConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FItemCollectionMetrics ItemCollectionMetrics;
};

USTRUCT(BlueprintType)
struct FDeleteTableResult
{
	GENERATED_BODY()
public:
	FDeleteTableResult() = default;
#if !DISABLE_DYNAMODB
	FDeleteTableResult(const Aws::DynamoDB::Model::DeleteTableResult& Data)
		: TableDescription(Data.GetTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableDescription TableDescription;
};

USTRUCT(BlueprintType)
struct FDescribeBackupResult
{
	GENERATED_BODY()
public:
	FDescribeBackupResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeBackupResult(const Aws::DynamoDB::Model::DescribeBackupResult& Data)
		: BackupDescription(Data.GetBackupDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FBackupDescription BackupDescription;
};

USTRUCT(BlueprintType)
struct FPointInTimeRecoveryDescription
{
	GENERATED_BODY()
public:
	FPointInTimeRecoveryDescription()
		: PointInTimeRecoveryStatus(EPointInTimeRecoveryStatus::NOT_SET)
		  , EarliestRestorableDateTime(0)
		  , LatestRestorableDateTime(0)
	{
	}
#if !DISABLE_DYNAMODB
	FPointInTimeRecoveryDescription(const Aws::DynamoDB::Model::PointInTimeRecoveryDescription& Data)
		: PointInTimeRecoveryStatus(static_cast<EPointInTimeRecoveryStatus>(Data.GetPointInTimeRecoveryStatus()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetEarliestRestorableDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), EarliestRestorableDateTime);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLatestRestorableDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LatestRestorableDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EPointInTimeRecoveryStatus PointInTimeRecoveryStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime EarliestRestorableDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime LatestRestorableDateTime;
};

USTRUCT(BlueprintType)
struct FContinuousBackupsDescription
{
	GENERATED_BODY()
public:
	FContinuousBackupsDescription()
		: ContinuousBackupsStatus(EContinuousBackupsStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FContinuousBackupsDescription(const Aws::DynamoDB::Model::ContinuousBackupsDescription& Data)
		: ContinuousBackupsStatus(static_cast<EContinuousBackupsStatus>(Data.GetContinuousBackupsStatus()))
		  , PointInTimeRecoveryDescription(Data.GetPointInTimeRecoveryDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EContinuousBackupsStatus ContinuousBackupsStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FPointInTimeRecoveryDescription PointInTimeRecoveryDescription;
};

USTRUCT(BlueprintType)
struct FDescribeContinuousBackupsResult
{
	GENERATED_BODY()
public:
	FDescribeContinuousBackupsResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeContinuousBackupsResult(const Aws::DynamoDB::Model::DescribeContinuousBackupsResult& Data)
		: ContinuousBackupsDescription(Data.GetContinuousBackupsDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FContinuousBackupsDescription ContinuousBackupsDescription;
};

USTRUCT(BlueprintType)
struct FFailureException
{
	GENERATED_BODY()
public:
	FFailureException() = default;
#if !DISABLE_DYNAMODB
	FFailureException(const Aws::DynamoDB::Model::FailureException& Data)
		: ExceptionName(UTF8_TO_TCHAR(Data.GetExceptionName().c_str()))
		  , ExceptionDescription(UTF8_TO_TCHAR(Data.GetExceptionDescription().c_str()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ExceptionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ExceptionDescription;
};

USTRUCT(BlueprintType)
struct FDescribeContributorInsightsResult
{
	GENERATED_BODY()
public:
	FDescribeContributorInsightsResult()
		: ContributorInsightsStatus(EContributorInsightsStatus::NOT_SET)
		  , LastUpdateDateTime(0)
	{
	}
#if !DISABLE_DYNAMODB
	FDescribeContributorInsightsResult(const Aws::DynamoDB::Model::DescribeContributorInsightsResult& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , ContributorInsightsStatus(static_cast<EContributorInsightsStatus>(Data.GetContributorInsightsStatus()))
		  , FailureException(Data.GetFailureException())
	{
		for (auto& Element : Data.GetContributorInsightsRuleList())
			ContributorInsightsRuleList.Add(UTF8_TO_TCHAR(Element.c_str()));

		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastUpdateDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastUpdateDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> ContributorInsightsRuleList;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EContributorInsightsStatus ContributorInsightsStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime LastUpdateDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FFailureException FailureException;
};

USTRUCT(BlueprintType)
struct FEndpoint
{
	GENERATED_BODY()
public:
	FEndpoint() = default;
#if !DISABLE_DYNAMODB
	FEndpoint(const Aws::DynamoDB::Model::Endpoint& Data)
		: Address(UTF8_TO_TCHAR(Data.GetAddress().c_str()))
		  , CachePeriodInMinutes(LexToString(Data.GetCachePeriodInMinutes()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString Address;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString CachePeriodInMinutes;
};

USTRUCT(BlueprintType)
struct FDescribeEndpointsResult
{
	GENERATED_BODY()
public:
	FDescribeEndpointsResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeEndpointsResult(const Aws::DynamoDB::Model::DescribeEndpointsResult& Data)
	{
		for (auto& Element : Data.GetEndpoints())
			Endpoints.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FEndpoint> Endpoints;
};

USTRUCT(BlueprintType)
struct FDescribeGlobalTableResult
{
	GENERATED_BODY()
public:
	FDescribeGlobalTableResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeGlobalTableResult(const Aws::DynamoDB::Model::DescribeGlobalTableResult& Data)
		: GlobalTableDescription(Data.GetGlobalTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FGlobalTableDescription GlobalTableDescription;
};

USTRUCT(BlueprintType)
struct FAutoScalingTargetTrackingScalingPolicyConfigurationDescription
{
	GENERATED_BODY()
public:
	FAutoScalingTargetTrackingScalingPolicyConfigurationDescription()
		: bDisableScaleIn(false)
		  , ScaleInCooldown(0)
		  , ScaleOutCooldown(0)
	{
	}
#if !DISABLE_DYNAMODB
	FAutoScalingTargetTrackingScalingPolicyConfigurationDescription(const Aws::DynamoDB::Model::AutoScalingTargetTrackingScalingPolicyConfigurationDescription& Data)
		: bDisableScaleIn(Data.GetDisableScaleIn())
		  , ScaleInCooldown(Data.GetScaleInCooldown())
		  , ScaleOutCooldown(Data.GetScaleOutCooldown())
		  , TargetValue(LexToString(Data.GetTargetValue()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bDisableScaleIn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 ScaleInCooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 ScaleOutCooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TargetValue;
};

USTRUCT(BlueprintType)
struct FAutoScalingPolicyDescription
{
	GENERATED_BODY()
public:
	FAutoScalingPolicyDescription() = default;
#if !DISABLE_DYNAMODB
	FAutoScalingPolicyDescription(const Aws::DynamoDB::Model::AutoScalingPolicyDescription& Data)
		: PolicyName(UTF8_TO_TCHAR(Data.GetPolicyName().c_str()))
		  , TargetTrackingScalingPolicyConfiguration(Data.GetTargetTrackingScalingPolicyConfiguration())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString PolicyName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingTargetTrackingScalingPolicyConfigurationDescription TargetTrackingScalingPolicyConfiguration;
};

USTRUCT(BlueprintType)
struct FAutoScalingSettingsDescription
{
	GENERATED_BODY()
public:
	FAutoScalingSettingsDescription()
		: bAutoScalingDisabled(false)
	{
	}
#if !DISABLE_DYNAMODB
	FAutoScalingSettingsDescription(const Aws::DynamoDB::Model::AutoScalingSettingsDescription& Data)
		: MinimumUnits(LexToString(Data.GetMinimumUnits()))
		  , MaximumUnits(LexToString(Data.GetMaximumUnits()))
		  , bAutoScalingDisabled(Data.GetAutoScalingDisabled())
		  , AutoScalingRoleArn(UTF8_TO_TCHAR(Data.GetAutoScalingRoleArn().c_str()))
	{
		for (auto& Element : Data.GetScalingPolicies())
			ScalingPolicies.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString MinimumUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString MaximumUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bAutoScalingDisabled;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAutoScalingPolicyDescription> ScalingPolicies;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString AutoScalingRoleArn;
};

USTRUCT(BlueprintType)
struct FReplicaGlobalSecondaryIndexSettingsDescription
{
	GENERATED_BODY()
public:
	FReplicaGlobalSecondaryIndexSettingsDescription()
		: IndexStatus(EIndexStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FReplicaGlobalSecondaryIndexSettingsDescription(const Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndexSettingsDescription& Data)
		: IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , IndexStatus(static_cast<EIndexStatus>(Data.GetIndexStatus()))
		  , ProvisionedReadCapacityUnits(LexToString(Data.GetProvisionedReadCapacityUnits()))
		  , ProvisionedReadCapacityAutoScalingSettings(Data.GetProvisionedReadCapacityAutoScalingSettings())
		  , ProvisionedWriteCapacityUnits(LexToString(Data.GetProvisionedWriteCapacityUnits()))
		  , ProvisionedWriteCapacityAutoScalingSettings(Data.GetProvisionedWriteCapacityAutoScalingSettings())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EIndexStatus IndexStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProvisionedReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ProvisionedReadCapacityAutoScalingSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProvisionedWriteCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ProvisionedWriteCapacityAutoScalingSettings;
};

USTRUCT(BlueprintType)
struct FReplicaSettingsDescription
{
	GENERATED_BODY()
public:
	FReplicaSettingsDescription()
		: ReplicaStatus(EReplicaStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FReplicaSettingsDescription(const Aws::DynamoDB::Model::ReplicaSettingsDescription& Data)
		: RegionName(UTF8_TO_TCHAR(Data.GetRegionName().c_str()))
		  , ReplicaStatus(static_cast<EReplicaStatus>(Data.GetReplicaStatus()))
		  , ReplicaBillingModeSummary(Data.GetReplicaBillingModeSummary())
		  , ReplicaProvisionedReadCapacityUnits(LexToString(Data.GetReplicaProvisionedReadCapacityUnits()))
		  , ReplicaProvisionedReadCapacityAutoScalingSettings(Data.GetReplicaProvisionedReadCapacityAutoScalingSettings())
		  , ReplicaProvisionedWriteCapacityUnits(LexToString(Data.GetReplicaProvisionedWriteCapacityUnits()))
		  , ReplicaProvisionedWriteCapacityAutoScalingSettings(Data.GetReplicaProvisionedWriteCapacityAutoScalingSettings())
	{
		for (auto& Element : Data.GetReplicaGlobalSecondaryIndexSettings())
			ReplicaGlobalSecondaryIndexSettings.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReplicaStatus ReplicaStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FBillingModeSummary ReplicaBillingModeSummary;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReplicaProvisionedReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ReplicaProvisionedReadCapacityAutoScalingSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReplicaProvisionedWriteCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ReplicaProvisionedWriteCapacityAutoScalingSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaGlobalSecondaryIndexSettingsDescription> ReplicaGlobalSecondaryIndexSettings;
};

USTRUCT(BlueprintType)
struct FDescribeGlobalTableSettingsResult
{
	GENERATED_BODY()
public:
	FDescribeGlobalTableSettingsResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeGlobalTableSettingsResult(const Aws::DynamoDB::Model::DescribeGlobalTableSettingsResult& Data)
		: GlobalTableName(UTF8_TO_TCHAR(Data.GetGlobalTableName().c_str()))
	{
		for (auto& Element : Data.GetReplicaSettings())
			ReplicaSettings.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaSettingsDescription> ReplicaSettings;
};

USTRUCT(BlueprintType)
struct FDescribeLimitsResult
{
	GENERATED_BODY()
public:
	FDescribeLimitsResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeLimitsResult(const Aws::DynamoDB::Model::DescribeLimitsResult& Data)
		: AccountMaxReadCapacityUnits(LexToString(Data.GetAccountMaxReadCapacityUnits()))
		  , AccountMaxWriteCapacityUnits(LexToString(Data.GetAccountMaxWriteCapacityUnits()))
		  , TableMaxReadCapacityUnits(LexToString(Data.GetTableMaxReadCapacityUnits()))
		  , TableMaxWriteCapacityUnits(LexToString(Data.GetTableMaxWriteCapacityUnits()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString AccountMaxReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString AccountMaxWriteCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableMaxReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableMaxWriteCapacityUnits;
};

USTRUCT(BlueprintType)
struct FDescribeTableResult
{
	GENERATED_BODY()
public:
	FDescribeTableResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeTableResult(const Aws::DynamoDB::Model::DescribeTableResult& Data)
		: Table(Data.GetTable())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableDescription Table;
};

USTRUCT(BlueprintType)
struct FReplicaGlobalSecondaryIndexAutoScalingDescription
{
	GENERATED_BODY()
public:
	FReplicaGlobalSecondaryIndexAutoScalingDescription()
		: IndexStatus(EIndexStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FReplicaGlobalSecondaryIndexAutoScalingDescription(const Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndexAutoScalingDescription& Data)
		: IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , IndexStatus(static_cast<EIndexStatus>(Data.GetIndexStatus()))
		  , ProvisionedReadCapacityAutoScalingSettings(Data.GetProvisionedReadCapacityAutoScalingSettings())
		  , ProvisionedWriteCapacityAutoScalingSettings(Data.GetProvisionedWriteCapacityAutoScalingSettings())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EIndexStatus IndexStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ProvisionedReadCapacityAutoScalingSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ProvisionedWriteCapacityAutoScalingSettings;
};

USTRUCT(BlueprintType)
struct FReplicaAutoScalingDescription
{
	GENERATED_BODY()
public:
	FReplicaAutoScalingDescription()
		: ReplicaStatus(EReplicaStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FReplicaAutoScalingDescription(const Aws::DynamoDB::Model::ReplicaAutoScalingDescription& Data)
		: RegionName(UTF8_TO_TCHAR(Data.GetRegionName().c_str()))
		  , ReplicaProvisionedReadCapacityAutoScalingSettings(Data.GetReplicaProvisionedReadCapacityAutoScalingSettings())
		  , ReplicaProvisionedWriteCapacityAutoScalingSettings(Data.GetReplicaProvisionedWriteCapacityAutoScalingSettings())
		  , ReplicaStatus(static_cast<EReplicaStatus>(Data.GetReplicaStatus()))
	{
		for (auto& Element : Data.GetGlobalSecondaryIndexes())
			GlobalSecondaryIndexes.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaGlobalSecondaryIndexAutoScalingDescription> GlobalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ReplicaProvisionedReadCapacityAutoScalingSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsDescription ReplicaProvisionedWriteCapacityAutoScalingSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReplicaStatus ReplicaStatus;
};

USTRUCT(BlueprintType)
struct FTableAutoScalingDescription
{
	GENERATED_BODY()
public:
	FTableAutoScalingDescription()
		: TableStatus(ETableStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FTableAutoScalingDescription(const Aws::DynamoDB::Model::TableAutoScalingDescription& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , TableStatus(static_cast<ETableStatus>(Data.GetTableStatus()))
	{
		for (auto& Element : Data.GetReplicas())
			Replicas.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ETableStatus TableStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaAutoScalingDescription> Replicas;
};

USTRUCT(BlueprintType)
struct FDescribeTableReplicaAutoScalingResult
{
	GENERATED_BODY()
public:
	FDescribeTableReplicaAutoScalingResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeTableReplicaAutoScalingResult(const Aws::DynamoDB::Model::DescribeTableReplicaAutoScalingResult& Data)
		: TableAutoScalingDescription(Data.GetTableAutoScalingDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableAutoScalingDescription TableAutoScalingDescription;
};

USTRUCT(BlueprintType)
struct FDescribeTimeToLiveResult
{
	GENERATED_BODY()
public:
	FDescribeTimeToLiveResult() = default;
#if !DISABLE_DYNAMODB
	FDescribeTimeToLiveResult(const Aws::DynamoDB::Model::DescribeTimeToLiveResult& Data)
		: TimeToLiveDescription(Data.GetTimeToLiveDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTimeToLiveDescription TimeToLiveDescription;
};

USTRUCT(BlueprintType)
struct FGetItemResult
{
	GENERATED_BODY()
public:
	FGetItemResult() = default;
#if !DISABLE_DYNAMODB
	FGetItemResult(const Aws::DynamoDB::Model::GetItemResult& Data)
		: ConsumedCapacity(Data.GetConsumedCapacity())
	{
		for (auto& Element : Data.GetItem())
			Item.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Item;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FConsumedCapacity ConsumedCapacity;
};

USTRUCT(BlueprintType)
struct FBackupSummary
{
	GENERATED_BODY()
public:
	FBackupSummary()
		: BackupCreationDateTime(0)
		  , BackupExpiryDateTime(0)
		  , BackupStatus(EBackupStatus::NOT_SET)
		  , BackupType(EBackupType::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FBackupSummary(const Aws::DynamoDB::Model::BackupSummary& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , TableId(UTF8_TO_TCHAR(Data.GetTableId().c_str()))
		  , TableArn(UTF8_TO_TCHAR(Data.GetTableArn().c_str()))
		  , BackupArn(UTF8_TO_TCHAR(Data.GetBackupArn().c_str()))
		  , BackupName(UTF8_TO_TCHAR(Data.GetBackupName().c_str()))
		  , BackupStatus(static_cast<EBackupStatus>(Data.GetBackupStatus()))
		  , BackupType(static_cast<EBackupType>(Data.GetBackupType()))
		  , BackupSizeBytes(LexToString(Data.GetBackupSizeBytes()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetBackupCreationDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), BackupCreationDateTime);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetBackupExpiryDateTime().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), BackupExpiryDateTime);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime BackupCreationDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime BackupExpiryDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBackupStatus BackupStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBackupType BackupType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupSizeBytes;
};

USTRUCT(BlueprintType)
struct FListBackupsResult
{
	GENERATED_BODY()
public:
	FListBackupsResult() = default;
#if !DISABLE_DYNAMODB
	FListBackupsResult(const Aws::DynamoDB::Model::ListBackupsResult& Data)
		: LastEvaluatedBackupArn(UTF8_TO_TCHAR(Data.GetLastEvaluatedBackupArn().c_str()))
	{
		for (auto& Element : Data.GetBackupSummaries())
			BackupSummaries.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FBackupSummary> BackupSummaries;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString LastEvaluatedBackupArn;
};

USTRUCT(BlueprintType)
struct FContributorInsightsSummary
{
	GENERATED_BODY()
public:
	FContributorInsightsSummary()
		: ContributorInsightsStatus(EContributorInsightsStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FContributorInsightsSummary(const Aws::DynamoDB::Model::ContributorInsightsSummary& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , ContributorInsightsStatus(static_cast<EContributorInsightsStatus>(Data.GetContributorInsightsStatus()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EContributorInsightsStatus ContributorInsightsStatus;
};


USTRUCT(BlueprintType)
struct FListContributorInsightsResult
{
	GENERATED_BODY()
public:
	FListContributorInsightsResult() = default;
#if !DISABLE_DYNAMODB
	FListContributorInsightsResult(const Aws::DynamoDB::Model::ListContributorInsightsResult& Data)
		: NextToken(UTF8_TO_TCHAR(Data.GetNextToken().c_str()))
	{
		for (auto& Element : Data.GetContributorInsightsSummaries())
			ContributorInsightsSummaries.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FContributorInsightsSummary> ContributorInsightsSummaries;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString NextToken;
};

USTRUCT(BlueprintType)
struct FGlobalTable
{
	GENERATED_BODY()
public:
	FGlobalTable() = default;
#if !DISABLE_DYNAMODB
	FGlobalTable(const Aws::DynamoDB::Model::GlobalTable& Data)
		: GlobalTableName(UTF8_TO_TCHAR(Data.GetGlobalTableName().c_str()))
	{
		for (auto& Element : Data.GetReplicationGroup())
			ReplicationGroup.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplica> ReplicationGroup;
};

USTRUCT(BlueprintType)
struct FListGlobalTablesResult
{
	GENERATED_BODY()
public:
	FListGlobalTablesResult() = default;
#if !DISABLE_DYNAMODB
	FListGlobalTablesResult(const Aws::DynamoDB::Model::ListGlobalTablesResult& Data)
		: LastEvaluatedGlobalTableName(UTF8_TO_TCHAR(Data.GetLastEvaluatedGlobalTableName().c_str()))
	{
		for (auto& Element : Data.GetGlobalTables())
			GlobalTables.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalTable> GlobalTables;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString LastEvaluatedGlobalTableName;
};

USTRUCT(BlueprintType)
struct FListTablesResult
{
	GENERATED_BODY()
public:
	FListTablesResult() = default;
#if !DISABLE_DYNAMODB
	FListTablesResult(const Aws::DynamoDB::Model::ListTablesResult& Data)
		: LastEvaluatedTableName(UTF8_TO_TCHAR(Data.GetLastEvaluatedTableName().c_str()))
	{
		for (auto& Element : Data.GetTableNames())
			TableNames.Add(UTF8_TO_TCHAR(Element.c_str()));
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> TableNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString LastEvaluatedTableName;
};

USTRUCT(BlueprintType)
struct FListTagsOfResourceResult
{
	GENERATED_BODY()
public:
	FListTagsOfResourceResult() = default;
#if !DISABLE_DYNAMODB
	FListTagsOfResourceResult(const Aws::DynamoDB::Model::ListTagsOfResourceResult& Data)
		: NextToken(UTF8_TO_TCHAR(Data.GetNextToken().c_str()))
	{
		for (auto& Element : Data.GetTags())
			Tags.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FDynamoDBTag> Tags;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString NextToken;
};

USTRUCT(BlueprintType)
struct FPutItemResult
{
	GENERATED_BODY()
public:
	FPutItemResult() = default;
#if !DISABLE_DYNAMODB
	FPutItemResult(const Aws::DynamoDB::Model::PutItemResult& Data)
		: ConsumedCapacity(Data.GetConsumedCapacity())
		  , ItemCollectionMetrics(Data.GetItemCollectionMetrics())
	{
		for (auto& Element : Data.GetAttributes())
			Attributes.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Attributes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FConsumedCapacity ConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FItemCollectionMetrics ItemCollectionMetrics;
};

USTRUCT(BlueprintType)
struct FAttributeValueMap
{
	GENERATED_BODY()
public:
	FAttributeValueMap() = default;

	FAttributeValueMap(FString key, FAttributeValue value)
	{
		Values.Add(key, value);
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Values;
};

USTRUCT(BlueprintType)
struct FQueryResult
{
	GENERATED_BODY()
public:
	FQueryResult()
		: Count(0)
		  , ScannedCount(0)
	{
	}
#if !DISABLE_DYNAMODB
	FQueryResult(const Aws::DynamoDB::Model::QueryResult& Data)
		: Count(Data.GetCount())
		  , ScannedCount(Data.GetScannedCount())
		  , ConsumedCapacity(Data.GetConsumedCapacity())
	{
		for (auto& Element : Data.GetItems())
		{
			FAttributeValueMap AttributeValueMap;

			for (auto& MappedElement : Element)
			{
				AttributeValueMap.Values.Add(UTF8_TO_TCHAR(MappedElement.first.c_str()), MappedElement.second);
			}

			Items.Add(AttributeValueMap);
		}

		for (auto& Element : Data.GetLastEvaluatedKey())
			LastEvaluatedKey.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeValueMap> Items;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Count;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 ScannedCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> LastEvaluatedKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FConsumedCapacity ConsumedCapacity;
};

USTRUCT(BlueprintType)
struct FRestoreTableFromBackupResult
{
	GENERATED_BODY()
public:
	FRestoreTableFromBackupResult() = default;
#if !DISABLE_DYNAMODB
	FRestoreTableFromBackupResult(const Aws::DynamoDB::Model::RestoreTableFromBackupResult& Data)
		: TableDescription(Data.GetTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableDescription TableDescription;
};

USTRUCT(BlueprintType)
struct FRestoreTableToPointInTimeResult
{
	GENERATED_BODY()
public:
	FRestoreTableToPointInTimeResult() = default;
#if !DISABLE_DYNAMODB
	FRestoreTableToPointInTimeResult(const Aws::DynamoDB::Model::RestoreTableToPointInTimeResult& Data)
		: TableDescription(Data.GetTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableDescription TableDescription;
};

USTRUCT(BlueprintType)
struct FScanResult
{
	GENERATED_BODY()
public:
	FScanResult()
		: Count(0)
		  , ScannedCount(0)
	{
	}
#if !DISABLE_DYNAMODB
	FScanResult(const Aws::DynamoDB::Model::ScanResult& Data)
		: Count(Data.GetCount())
		  , ScannedCount(Data.GetScannedCount())
		  , ConsumedCapacity(Data.GetConsumedCapacity())
	{
		for (auto& Element : Data.GetItems())
		{
			FAttributeValueMap AttributeValueMap;

			for (auto& MappedElement : Element)
			{
				AttributeValueMap.Values.Add(UTF8_TO_TCHAR(MappedElement.first.c_str()), MappedElement.second);
			}

			Items.Add(AttributeValueMap);
		}

		for (auto& Element : Data.GetLastEvaluatedKey())
		{
			LastEvaluatedKey.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
		}
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeValueMap> Items;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Count;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 ScannedCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> LastEvaluatedKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FConsumedCapacity ConsumedCapacity;
};

USTRUCT(BlueprintType)
struct FItemResponse
{
	GENERATED_BODY()
public:
	FItemResponse() = default;
#if !DISABLE_DYNAMODB
	FItemResponse(const Aws::DynamoDB::Model::ItemResponse& Data)
	{
		for (auto& Element : Data.GetItem())
			Item.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Item;
};

USTRUCT(BlueprintType)
struct FTransactGetItemsResult
{
	GENERATED_BODY()
public:
	FTransactGetItemsResult() = default;
#if !DISABLE_DYNAMODB
	FTransactGetItemsResult(const Aws::DynamoDB::Model::TransactGetItemsResult& Data)
	{
		for (auto& Element : Data.GetConsumedCapacity())
			ConsumedCapacity.Add(Element);

		for (auto& Element : Data.GetResponses())
			Responses.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FConsumedCapacity> ConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FItemResponse> Responses;
};

USTRUCT(BlueprintType)
struct FItemCollectionMetricsArray
{
	GENERATED_BODY()
public:
	FItemCollectionMetricsArray() = default;

	FItemCollectionMetricsArray(TArray<FItemCollectionMetrics> values)
		: Items(values)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FItemCollectionMetrics> Items;
};

USTRUCT(BlueprintType)
struct FTransactWriteItemsResult
{
	GENERATED_BODY()
public:
	FTransactWriteItemsResult() = default;
#if !DISABLE_DYNAMODB
	FTransactWriteItemsResult(const Aws::DynamoDB::Model::TransactWriteItemsResult& Data)
	{
		for (auto& Element : Data.GetConsumedCapacity())
			ConsumedCapacity.Add(Element);

		for (auto& Element : Data.GetItemCollectionMetrics())
		{
			TArray<FItemCollectionMetrics> ItemCollectionMetricsArray;

			for (auto& ItemCollectionMetricsElement : Element.second)
			{
				ItemCollectionMetricsArray.Add(ItemCollectionMetricsElement);
			}

			ItemCollectionMetrics.Add(UTF8_TO_TCHAR(Element.first.c_str()), ItemCollectionMetricsArray);
		}
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FConsumedCapacity> ConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FItemCollectionMetricsArray> ItemCollectionMetrics;
};

USTRUCT(BlueprintType)
struct FUpdateContinuousBackupsResult
{
	GENERATED_BODY()
public:
	FUpdateContinuousBackupsResult() = default;
#if !DISABLE_DYNAMODB
	FUpdateContinuousBackupsResult(const Aws::DynamoDB::Model::UpdateContinuousBackupsResult& Data)
		: ContinuousBackupsDescription(Data.GetContinuousBackupsDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FContinuousBackupsDescription ContinuousBackupsDescription;
};

USTRUCT(BlueprintType)
struct FUpdateContributorInsightsResult
{
	GENERATED_BODY()
public:
	FUpdateContributorInsightsResult()
		: ContributorInsightsStatus(EContributorInsightsStatus::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FUpdateContributorInsightsResult(const Aws::DynamoDB::Model::UpdateContributorInsightsResult& Data)
		: TableName(UTF8_TO_TCHAR(Data.GetTableName().c_str()))
		  , IndexName(UTF8_TO_TCHAR(Data.GetIndexName().c_str()))
		  , ContributorInsightsStatus(static_cast<EContributorInsightsStatus>(Data.GetContributorInsightsStatus()))
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EContributorInsightsStatus ContributorInsightsStatus;
};

USTRUCT(BlueprintType)
struct FUpdateGlobalTableResult
{
	GENERATED_BODY()
public:
	FUpdateGlobalTableResult() = default;
#if !DISABLE_DYNAMODB
	FUpdateGlobalTableResult(const Aws::DynamoDB::Model::UpdateGlobalTableResult& Data)
		: GlobalTableDescription(Data.GetGlobalTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FGlobalTableDescription GlobalTableDescription;
};

USTRUCT(BlueprintType)
struct FUpdateGlobalTableSettingsResult
{
	GENERATED_BODY()
public:
	FUpdateGlobalTableSettingsResult() = default;
#if !DISABLE_DYNAMODB
	FUpdateGlobalTableSettingsResult(const Aws::DynamoDB::Model::UpdateGlobalTableSettingsResult& Data)
		: GlobalTableName(UTF8_TO_TCHAR(Data.GetGlobalTableName().c_str()))
	{
		for (auto& Element : Data.GetReplicaSettings())
			ReplicaSettings.Add(Element);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaSettingsDescription> ReplicaSettings;
};

USTRUCT(BlueprintType)
struct FUpdateItemResult
{
	GENERATED_BODY()
public:
	FUpdateItemResult() = default;
#if !DISABLE_DYNAMODB
	FUpdateItemResult(const Aws::DynamoDB::Model::UpdateItemResult& Data)
		: ConsumedCapacity(Data.GetConsumedCapacity())
		  , ItemCollectionMetrics(Data.GetItemCollectionMetrics())
	{
		for (auto& Element : Data.GetAttributes())
			Attributes.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Attributes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FConsumedCapacity ConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FItemCollectionMetrics ItemCollectionMetrics;
};

USTRUCT(BlueprintType)
struct FUpdateTableResult
{
	GENERATED_BODY()
public:
	FUpdateTableResult() = default;
#if !DISABLE_DYNAMODB
	FUpdateTableResult(const Aws::DynamoDB::Model::UpdateTableResult& Data)
		: TableDescription(Data.GetTableDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableDescription TableDescription;
};

USTRUCT(BlueprintType)
struct FUpdateTableReplicaAutoScalingResult
{
	GENERATED_BODY()
public:
	FUpdateTableReplicaAutoScalingResult() = default;
#if !DISABLE_DYNAMODB
	FUpdateTableReplicaAutoScalingResult(const Aws::DynamoDB::Model::UpdateTableReplicaAutoScalingResult& Data)
		: TableAutoScalingDescription(Data.GetTableAutoScalingDescription())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTableAutoScalingDescription TableAutoScalingDescription;
};

USTRUCT(BlueprintType)
struct FUpdateTimeToLiveResult
{
	GENERATED_BODY()
public:
	FUpdateTimeToLiveResult() = default;
#if !DISABLE_DYNAMODB
	FUpdateTimeToLiveResult(const Aws::DynamoDB::Model::UpdateTimeToLiveResult& Data)
		: TimeToLiveSpecification(Data.GetTimeToLiveSpecification())
	{
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTimeToLiveSpecification TimeToLiveSpecification;
};

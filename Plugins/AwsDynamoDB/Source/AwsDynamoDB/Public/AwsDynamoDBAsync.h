/**
* Copyright (C) 2017-2022 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "AwsDynamoDBTypes.h"
#include "AwsDynamoDBLibrary.h"
#include "AwsDynamoDBResultTypes.h"
#include <Kismet/BlueprintAsyncActionBase.h>
#include "AwsDynamoDBAsync.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoBatchGetItem
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnBatchGetItemCallback, bool, bSuccess, const FBatchGetItemResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoBatchGetItem : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnBatchGetItemCallback Callback;
public:
	/**
	* The BatchGetItem operation returns the attributes of one or more items from one or more tables. You identify requested items by primary key.
	*
	* A single operation can retrieve up to 16 MB of data, which can contain as many as 100 items. BatchGetItem returns a partial result if the response size limit is exceeded, the table's provisioned throughput is exceeded, or an internal processing failure occurs. If a partial result is returned, the operation returns a value for UnprocessedKeys. You can use this value to retry the operation starting with the next item to get.
	* For example, if you ask to retrieve 100 items, but each individual item is 300 KB in size, the system returns 52 items (so as not to exceed the 16 MB limit). It also returns an appropriate UnprocessedKeys value so you can get the next page of results. If desired, your application can include its own logic to assemble the pages of results into one dataset.
	* If none of the items can be processed due to insufficient provisioned throughput on all of the tables in the request, then BatchGetItem returns a ProvisionedThroughputExceededException. If at least one of the items is successfully processed, then BatchGetItem completes successfully, while returning the keys of the unread items in UnprocessedKeys.
	*
	* By default, BatchGetItem performs eventually consistent reads on every table in the request. If you want strongly consistent reads instead, you can set ConsistentRead to true for any or all tables.
	* 
	* In order to minimize response latency, BatchGetItem retrieves items in parallel.
	* When designing your application, keep in mind that DynamoDB does not return items in any particular order. To help parse the response by item, include the primary key values for the items in your request in the ProjectionExpression parameter.
	* If a requested item does not exist, it is not returned in the result. Requests for nonexistent items consume the minimum read capacity units according to the type of read. For more information, see Working with Tables in the Amazon DynamoDB Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoBatchGetItem* BatchGetItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FBatchGetItemRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;

	FBatchGetItemRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoBatchWriteItem
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnBatchWriteItemCallback, bool, bSuccess, const FBatchWriteItemResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoBatchWriteItem : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnBatchWriteItemCallback Callback;
public:
	/**
	* The BatchWriteItem operation puts or deletes multiple items in one or more tables. 
	* A single call to BatchWriteItem can write up to 16 MB of data, which can comprise as many as 25 put or delete requests. Individual items to be written can be as large as 400 KB.
	*
	* The individual PutItem and DeleteItem operations specified in BatchWriteItem are atomic; however BatchWriteItem as a whole is not. If any requested operations fail because the table's provisioned throughput is exceeded or an internal processing failure occurs, the failed operations are returned in the UnprocessedItems response parameter. You can investigate and optionally resend the requests. Typically, you would call BatchWriteItem in a loop. Each iteration would check for unprocessed items and submit a new BatchWriteItem request with those unprocessed items until all items have been processed.
	* 
	* If none of the items can be processed due to insufficient provisioned throughput on all of the tables in the request, then BatchWriteItem returns a ProvisionedThroughputExceededException.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoBatchWriteItem* BatchWriteItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FBatchWriteItemRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	
	FBatchWriteItemRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoCreateBackup
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreateBackupCallback, bool, bSuccess, const FCreateBackupResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoCreateBackup : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCreateBackupCallback Callback;
public:
	/**
	* Creates a backup for an existing table.
	*
	* Each time you create an on-demand backup, the entire table data is backed up. There is no limit to the number of on-demand backups that can be taken.
	* When you create an on-demand backup, a time marker of the request is cataloged, and the backup is created asynchronously, by applying all changes until the time of the request to the last full table snapshot. Backup requests are processed instantaneously and become available for restore within minutes.
	* You can call CreateBackup at a maximum rate of 50 times per second.
	* All backups in DynamoDB work without consuming any provisioned throughput on the table.
	* If you submit a backup request on 2018-12-14 at 14:25:00, the backup is guaranteed to contain all data committed to the table up to 14:24:00, and data committed after 14:26:00 will not be. The backup might contain data modifications made between 14:24:00 and 14:26:00. On-demand backup does not support causal consistency.
	*
	* Along with data, the following are also included on the backups:
	* Global secondary indexes (GSIs)
	* Local secondary indexes (LSIs)
	* Streams
	* Provisioned read and write capacity
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoCreateBackup* CreateBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FCreateBackupRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FCreateBackupRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoCreateGlobalTable
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreateGlobalTableCallback, bool, bSuccess, const FCreateGlobalTableResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoCreateGlobalTable : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCreateGlobalTableCallback Callback;
public:
	/**
	* Creates a global table from an existing table. 
	* A global table creates a replication relationship between two or more DynamoDB tables with the same table name in the provided Regions.
	* 
	* If you want to add a new replica table to a global table, each of the following conditions must be true:
	*
	* The table must have the same primary key as all of the other replicas.
	* The table must have the same name as all of the other replicas.
	* The table must have DynamoDB Streams enabled, with the stream containing both the new and the old images of the item.
	* None of the replica tables in the global table can contain any data.
	*
	* If global secondary indexes are specified, then the following conditions must also be met:
	* The global secondary indexes must have the same name.
	* The global secondary indexes must have the same hash key and sort key (if present).
	* If local secondary indexes are specified, then the following conditions must also be met:
	* The local secondary indexes must have the same name.
	* The local secondary indexes must have the same hash key and sort key (if present).
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoCreateGlobalTable* CreateGlobalTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FCreateGlobalTableRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FCreateGlobalTableRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoCreateTable
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreateTableCallback, bool, bSuccess, const FCreateTableResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoCreateTable : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCreateTableCallback Callback;
public:
	/**
	* The CreateTable operation adds a new table to your account. In an AWS account, table names must be unique within each Region. That is, you can have two tables with same name if you create the tables in different Regions.
	*
	* CreateTable is an asynchronous operation. Upon receiving a CreateTable request, DynamoDB immediately returns a response with a TableStatus of CREATING. After the table is created, DynamoDB sets the TableStatus to ACTIVE. You can perform read and write operations only on an ACTIVE table.
	* You can optionally define secondary indexes on the new table, as part of the CreateTable operation. If you want to create multiple tables with secondary indexes on them, you must create the tables sequentially. Only one table with secondary indexes can be in the CREATING state at any given time.
	* You can use the DescribeTable action to check the table status.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoCreateTable* CreateTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FCreateTableRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FCreateTableRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDeleteBackup
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDeleteBackupCallback, bool, bSuccess, const FDeleteBackupResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDeleteBackup : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBackupCallback Callback;
public:
	/**
	* Deletes an existing backup of a table.
	*
	* You can call DeleteBackup at a maximum rate of 10 times per second.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDeleteBackup* DeleteBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDeleteBackupRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDeleteBackupRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDeleteItem
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDeleteItemCallback, bool, bSuccess, const FDeleteItemResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDeleteItem : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteItemCallback Callback;
public:
	/**
	* Deletes a single item in a table by primary key. You can perform a conditional delete operation that deletes the item if it exists, or if it has an expected attribute value.
	* In addition to deleting an item, you can also return the item's attribute values in the same operation, using the ReturnValues parameter.
	* Unless you specify conditions, the DeleteItem is an idempotent operation; running it multiple times on the same item or attribute does not result in an error response.
	* Conditional deletes are useful for deleting items only if specific conditions are met. If those conditions are met, DynamoDB performs the delete. Otherwise, the item is not deleted.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDeleteItem* DeleteItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDeleteItemRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDeleteItemRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDeleteTable
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDeleteTableCallback, bool, bSuccess, const FDeleteTableResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDeleteTable : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteTableCallback Callback;
public:
	/**
	* The DeleteTable operation deletes a table and all of its items. 
	* After a DeleteTable request, the specified table is in the DELETING state until DynamoDB completes the deletion. If the table is in the ACTIVE state, you can delete it. If a table is in CREATING or UPDATING states, then DynamoDB returns a ResourceInUseException. If the specified table does not exist, DynamoDB returns a ResourceNotFoundException. If table is already in the DELETING state, no error is returned.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDeleteTable* DeleteTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDeleteTableRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDeleteTableRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeBackup
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeBackupCallback, bool, bSuccess, const FDescribeBackupResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeBackup : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeBackupCallback Callback;
public:
	/**
	* Describes an existing backup of a table.
	* ou can call DescribeBackup at a maximum rate of 10 times per second.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeBackup* DescribeBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeBackupRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeBackupRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeContinuousBackups
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeContinuousBackupsCallback, bool, bSuccess, const FDescribeContinuousBackupsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeContinuousBackups : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeContinuousBackupsCallback Callback;
public:
	/**
	* Checks the status of continuous backups and point in time recovery on the specified table. Continuous backups are ENABLED on all tables at table creation. If point in time recovery is enabled, PointInTimeRecoveryStatus will be set to ENABLED.
	*
	* After continuous backups and point in time recovery are enabled, you can restore to any point in time within EarliestRestorableDateTime and LatestRestorableDateTime.
	* LatestRestorableDateTime is typically 5 minutes before the current time. You can restore your table to any point in time during the last 35 days.
	* You can call DescribeContinuousBackups at a maximum rate of 10 times per second.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeContinuousBackups* DescribeContinuousBackups(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeContinuousBackupsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeContinuousBackupsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeContributorInsights
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeContributorInsightsCallback, bool, bSuccess, const FDescribeContributorInsightsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeContributorInsights : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeContributorInsightsCallback Callback;
public:
	/**
	* Returns information about contributor insights, for a given table or global secondary index.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeContributorInsights* DescribeContributorInsights(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeContributorInsightsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeContributorInsightsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeEndpoints
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeEndpointsCallback, bool, bSuccess, const FDescribeEndpointsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeEndpoints : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeEndpointsCallback Callback;
public:
	/**
	* Returns the regional endpoint information.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeEndpoints* DescribeEndpoints(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeEndpointsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeEndpointsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeGlobalTable
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeGlobalTableCallback, bool, bSuccess, const FDescribeGlobalTableResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeGlobalTable : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeGlobalTableCallback Callback;
public:
	/**
	* Returns information about the specified global table.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeGlobalTable* DescribeGlobalTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeGlobalTableRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeGlobalTableRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeGlobalTableSettings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeGlobalTableSettingsCallback, bool, bSuccess, const FDescribeGlobalTableSettingsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeGlobalTableSettings : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeGlobalTableSettingsCallback Callback;
public:
	/**
	* Describes Region-specific settings for a global table.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeGlobalTableSettings* DescribeGlobalTableSettings(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeGlobalTableSettingsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeGlobalTableSettingsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeLimits
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeLimitsCallback, bool, bSuccess, const FDescribeLimitsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeLimits : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeLimitsCallback Callback;
public:
	/**
	* Returns the current provisioned-capacity quotas for your AWS account in a Region, both for the Region as a whole and for any one DynamoDB table that you create there.
	*
	* When you establish an AWS account, the account has initial quotas on the maximum read capacity units and write capacity units that you can provision across all of your DynamoDB tables in a given Region. Also, there are per-table quotas that apply when you create a table there. For more information, see Service, Account, and Table Quotas page in the Amazon DynamoDB Developer Guide.
	* Although you can increase these quotas by filing a case at AWS Support Center, obtaining the increase is not instantaneous. The DescribeLimits action lets you write code to compare the capacity you are currently using to those quotas imposed by your account so that you have enough time to apply for an increase before you hit a quota.
	* For example, you could use one of the AWS SDKs to do the following:
	* Call DescribeLimits for a particular Region to obtain your current account quotas on provisioned capacity there.
	* Create a variable to hold the aggregate read capacity units provisioned for all your tables in that Region, and one to hold the aggregate write capacity units. Zero them both.
	* Call ListTables to obtain a list of all your DynamoDB tables.
	*
	* For each table name listed by ListTables, do the following:
	* Call DescribeTable with the table name.
	* Use the data returned by DescribeTable to add the read capacity units and write capacity units provisioned for the table itself to your variables.
	* If the table has one or more global secondary indexes (GSIs), loop over these GSIs and add their provisioned capacity values to your variables as well.
	* Report the account quotas for that Region returned by DescribeLimits, along with the total current provisioned capacity levels you have calculated.
	* This will let you see whether you are getting close to your account-level quotas.
	* The per-table quotas apply only when you are creating a new table. They restrict the sum of the provisioned capacity of the new table itself and all its global secondary indexes.
	* For existing tables and their GSIs, DynamoDB doesn't let you increase provisioned capacity extremely rapidly, but the only quota that applies is that the aggregate provisioned capacity over all your tables and GSIs cannot exceed either of the per-account quotas.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeLimits* DescribeLimits(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeLimitsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeLimitsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeTable
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeTableCallback, bool, bSuccess, const FDescribeTableResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeTable : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeTableCallback Callback;
public:
	/**
	* Returns information about the table, including the current status of the table, when it was created, the primary key schema, and any indexes on the table.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeTable* DescribeTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeTableRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeTableRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeTableReplicaAutoScaling
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeTableReplicaAutoScalingCallback, bool, bSuccess, const FDescribeTableReplicaAutoScalingResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeTableReplicaAutoScaling : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeTableReplicaAutoScalingCallback Callback;
public:
	/**
	* Describes auto scaling settings across replicas of the global table at once.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeTableReplicaAutoScaling* DescribeTableReplicaAutoScaling(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeTableReplicaAutoScalingRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeTableReplicaAutoScalingRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoDescribeTimeToLive
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDescribeTimeToLiveCallback, bool, bSuccess, const FDescribeTimeToLiveResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoDescribeTimeToLive : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDescribeTimeToLiveCallback Callback;
public:
	/**
	* Gives a description of the Time to Live (TTL) status on the specified table.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoDescribeTimeToLive* DescribeTimeToLive(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FDescribeTimeToLiveRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FDescribeTimeToLiveRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoGetItem
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetItemCallback, bool, bSuccess, const FGetItemResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoGetItem : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetItemCallback Callback;
public:
	/**
	* The GetItem operation returns a set of attributes for the item with the given primary key. If there is no matching item, GetItem does not return any data and there will be no Item element in the response.
	*
	* GetItem provides an eventually consistent read by default. If your application requires a strongly consistent read, set ConsistentRead to true. Although a strongly consistent read might take more time than an eventually consistent read, it always returns the last updated value.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoGetItem* GetItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FGetItemRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FGetItemRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoListBackups
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListBackupsCallback, bool, bSuccess, const FListBackupsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoListBackups : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListBackupsCallback Callback;
public:
	/**
	* List backups associated with an AWS account. To list backups for a given table, specify TableName. ListBackups returns a paginated list of results with at most 1 MB worth of items in a page. You can also specify a maximum number of entries to be returned in a page.
	*
	* In the request, start time is inclusive, but end time is exclusive. Note that these boundaries are for the time at which the original backup was requested.
	* You can call ListBackups a maximum of five times per second.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoListBackups* ListBackups(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListBackupsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FListBackupsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoListContributorInsights
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListContributorInsightsCallback, bool, bSuccess, const FListContributorInsightsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoListContributorInsights : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListContributorInsightsCallback Callback;
public:
	/**
	* Returns a list of ContributorInsightsSummary for a table and all its global secondary indexes.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoListContributorInsights* ListContributorInsights(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListContributorInsightsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FListContributorInsightsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoListGlobalTables
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListGlobalTablesCallback, bool, bSuccess, const FListGlobalTablesResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoListGlobalTables : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListGlobalTablesCallback Callback;
public:
	/**
	* Lists all global tables that have a replica in the specified Region.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoListGlobalTables* ListGlobalTables(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListGlobalTablesRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FListGlobalTablesRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoListTables
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListTablesCallback, bool, bSuccess, const FListTablesResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoListTables : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListTablesCallback Callback;
public:
	/**
	* Returns an array of table names associated with the current account and endpoint. The output from ListTables is paginated, with each page returning a maximum of 100 table names.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoListTables* ListTables(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListTablesRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FListTablesRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoListTagsOfResource
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListTagsOfResourceCallback, bool, bSuccess, const FListTagsOfResourceResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoListTagsOfResource : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListTagsOfResourceCallback Callback;
public:
	/**
	* List all tags on an Amazon DynamoDB resource. You can call ListTagsOfResource up to 10 times per second, per account.
	* For an overview on tagging DynamoDB resources, see Tagging for DynamoDB in the Amazon DynamoDB Developer Guide.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoListTagsOfResource* ListTagsOfResource(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FListTagsOfResourceRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FListTagsOfResourceRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoPutItem
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPutItemCallback, bool, bSuccess, const FPutItemResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoPutItem : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutItemCallback Callback;
public:
	/**
	* Creates a new item, or replaces an old item with a new item. 
	*
	* If an item that has the same primary key as the new item already exists in the specified table, the new item completely replaces the existing item. You can perform a conditional put operation (add a new item if one with the specified primary key doesn't exist), or replace an existing item if it has certain attribute values. You can return the item's attribute values in the same operation, using the ReturnValues parameter.
	* When you add an item, the primary key attributes are the only required attributes. Attribute values cannot be null.
	* Empty String and Binary attribute values are allowed. Attribute values of type String and Binary must have a length greater than zero if the attribute is used as a key attribute for a table or index. Set type attributes cannot be empty.
	* Invalid Requests with empty values will be rejected with a ValidationException exception.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoPutItem* PutItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FPutItemRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FPutItemRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoQuery
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnQueryCallback, bool, bSuccess, const FQueryResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoQuery : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnQueryCallback Callback;
public:
	/**
	* The Query operation finds items based on primary key values. You can query any table or secondary index that has a composite primary key (a partition key and a sort key).
	*
	* Use the KeyConditionExpression parameter to provide a specific value for the partition key. The Query operation will return all of the items from the table or index with that partition key value. You can optionally narrow the scope of the Query operation by specifying a sort key value and a comparison operator in KeyConditionExpression. To further refine the Query results, you can optionally provide a FilterExpression. A FilterExpression determines which items within the results should be returned to you. All of the other results are discarded.
	* A Query operation always returns a result set. If no matching items are found, the result set will be empty. Queries that do not return results consume the minimum number of read capacity units for that type of read operation.
	* Query results are always sorted by the sort key value. If the data type of the sort key is Number, the results are returned in numeric order; otherwise, the results are returned in order of UTF-8 bytes. By default, the sort order is ascending. To reverse the order, set the ScanIndexForward parameter to false.
	* A single Query operation will read up to the maximum number of items set (if using the Limit parameter) or a maximum of 1 MB of data and then apply any filtering to the results using FilterExpression. If LastEvaluatedKey is present in the response, you will need to paginate the result set. For more information, see Paginating the Results in the Amazon DynamoDB Developer Guide.
	* FilterExpression is applied after a Query finishes, but before the results are returned. A FilterExpression cannot contain partition key or sort key attributes. You need to specify those attributes in the KeyConditionExpression.
	*
	* You can query a table, a local secondary index, or a global secondary index. For a query on a table or on a local secondary index, you can set the ConsistentRead parameter to true and obtain a strongly consistent result. Global secondary indexes support eventually consistent reads only, so do not specify ConsistentRead when querying a global secondary index.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoQuery* Query(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FQueryRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FQueryRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoRestoreTableFromBackup
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRestoreTableFromBackupCallback, bool, bSuccess, const FRestoreTableFromBackupResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoRestoreTableFromBackup : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnRestoreTableFromBackupCallback Callback;
public:
	/**
	* Creates a new table from an existing backup. Any number of users can execute up to 4 concurrent restores (any type of restore) in a given account.
	*
	* You can call RestoreTableFromBackup at a maximum rate of 10 times per second.
	* You must manually set up the following on the restored table:
	* Auto scaling policies
	* IAM policies
	* Amazon CloudWatch metrics and alarms
	* Tags
	* Stream settings
	* Time to Live (TTL) settings
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoRestoreTableFromBackup* RestoreTableFromBackup(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FRestoreTableFromBackupRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FRestoreTableFromBackupRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoRestoreTableToPointInTime
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRestoreTableToPointInTimeCallback, bool, bSuccess, const FRestoreTableToPointInTimeResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoRestoreTableToPointInTime : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnRestoreTableToPointInTimeCallback Callback;
public:
	/**
	* Restores the specified table to the specified point in time within EarliestRestorableDateTime and LatestRestorableDateTime. You can restore your table to any point in time during the last 35 days. Any number of users can execute up to 4 concurrent restores (any type of restore) in a given account.
	*
	* When you restore using point in time recovery, DynamoDB restores your table data to the state based on the selected date and time (day:hour:minute:second) to a new table.
	* Along with data, the following are also included on the new restored table using point in time recovery:
	* Global secondary indexes (GSIs)
	* Local secondary indexes (LSIs)
	* Provisioned read and write capacity
	* Encryption settings
	* 
	* You must manually set up the following on the restored table:
	* Auto scaling policies
	* IAM policies
	* Amazon CloudWatch metrics and alarms
	* Tags
	* Stream settings
	* Time to Live (TTL) settings
	* Point in time recovery settings
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoRestoreTableToPointInTime* RestoreTableToPointInTime(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FRestoreTableToPointInTimeRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FRestoreTableToPointInTimeRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoScan
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnScanCallback, bool, bSuccess, const FScanResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoScan : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnScanCallback Callback;
public:
	/**
	* The Scan operation returns one or more items and item attributes by accessing every item in a table or a secondary index. To have DynamoDB return fewer items, you can provide a FilterExpression operation.
	* 
	* If the total number of scanned items exceeds the maximum dataset size limit of 1 MB, the scan stops and results are returned to the user as a LastEvaluatedKey value to continue the scan in a subsequent operation. The results also include the number of items exceeding the limit. A scan can result in no table data meeting the filter criteria.
	* A single Scan operation reads up to the maximum number of items set (if using the Limit parameter) or a maximum of 1 MB of data and then apply any filtering to the results using FilterExpression. If LastEvaluatedKey is present in the response, you need to paginate the result set. For more information, see Paginating the Results in the Amazon DynamoDB Developer Guide.
	* Scan operations proceed sequentially; however, for faster performance on a large table or secondary index, applications can request a parallel Scan operation by providing the Segment and TotalSegments parameters. For more information, see Parallel Scan in the Amazon DynamoDB Developer Guide.
	* Scan uses eventually consistent reads when accessing the data in a table; therefore, the result set might not include the changes to data in the table immediately before the operation began. If you need a consistent copy of the data, as of the time that the Scan begins, you can set the ConsistentRead parameter to true.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoScan* Scan(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FScanRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FScanRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoTagResource
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTagResourceCallback, bool, bSuccess, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoTagResource : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnTagResourceCallback Callback;
public:
	/**
	* Associate a set of tags with an Amazon DynamoDB resource. You can then activate these user-defined tags so that they appear on the Billing and Cost Management console for cost allocation tracking. You can call TagResource up to five times per second, per account.
	*
	* For an overview on tagging DynamoDB resources, see Tagging for DynamoDB in the Amazon DynamoDB Developer Guide.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoTagResource* TagResource(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FTagResourceRequestDynamoDB& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FTagResourceRequestDynamoDB m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoTransactGetItems
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnTransactGetItemsCallback, bool, bSuccess, const FTransactGetItemsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoTransactGetItems : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnTransactGetItemsCallback Callback;
public:
	/**
	* TransactGetItems is a synchronous operation that atomically retrieves multiple items from one or more tables (but not from indexes) in a single account and Region. A TransactGetItems call can contain up to 25 TransactGetItem objects, each of which contains a Get structure that specifies an item to retrieve from a table in the account and Region. A call to TransactGetItems cannot retrieve items from tables in more than one AWS account or Region. The aggregate size of the items in the transaction cannot exceed 4 MB.
	*
	* DynamoDB rejects the entire TransactGetItems request if any of the following is true:
	* A conflicting operation is in the process of updating an item to be read.
	* There is insufficient provisioned capacity for the transaction to be completed.
	* There is a user error, such as an invalid data format.
	* The aggregate size of the items in the transaction cannot exceed 4 MB.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoTransactGetItems* TransactGetItems(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FTransactGetItemsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FTransactGetItemsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoTransactWriteItems
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnTransactWriteItemsCallback, bool, bSuccess, const FTransactWriteItemsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoTransactWriteItems : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnTransactWriteItemsCallback Callback;
public:
	/**
	* TransactWriteItems is a synchronous write operation that groups up to 25 action requests. These actions can target items in different tables, but not in different AWS accounts or Regions, and no two actions can target the same item. For example, you cannot both ConditionCheck and Update the same item. The aggregate size of the items in the transaction cannot exceed 4 MB.
	*
	* The actions are completed atomically so that either all of them succeed, or all of them fail. They are defined by the following objects:
	* Put  �   Initiates a PutItem operation to write a new item. This structure specifies the primary key of the item to be written, the name of the table to write it in, an optional condition expression that must be satisfied for the write to succeed, a list of the item's attributes, and a field indicating whether to retrieve the item's attributes if the condition is not met.
	* Update  �   Initiates an UpdateItem operation to update an existing item. This structure specifies the primary key of the item to be updated, the name of the table where it resides, an optional condition expression that must be satisfied for the update to succeed, an expression that defines one or more attributes to be updated, and a field indicating whether to retrieve the item's attributes if the condition is not met.
	* Delete  �   Initiates a DeleteItem operation to delete an existing item. This structure specifies the primary key of the item to be deleted, the name of the table where it resides, an optional condition expression that must be satisfied for the deletion to succeed, and a field indicating whether to retrieve the item's attributes if the condition is not met.
	* ConditionCheck  �   Applies a condition to an item that is not being modified by the transaction. This structure specifies the primary key of the item to be checked, the name of the table where it resides, a condition expression that must be satisfied for the transaction to succeed, and a field indicating whether to retrieve the item's attributes if the condition is not met.
	* 
	* DynamoDB rejects the entire TransactWriteItems request if any of the following is true:
	* A condition in one of the condition expressions is not met.
	* An ongoing operation is in the process of updating the same item.
	* There is insufficient provisioned capacity for the transaction to be completed.
	* An item size becomes too large (bigger than 400 KB), a local secondary index (LSI) becomes too large, or a similar validation error occurs because of changes made by the transaction.
	* The aggregate size of the items in the transaction exceeds 4 MB.
	* There is a user error, such as an invalid data format.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoTransactWriteItems* TransactWriteItems(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FTransactWriteItemsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FTransactWriteItemsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUntagResource
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUntagResourceCallback, bool, bSuccess, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUntagResource : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUntagResourceCallback Callback;
public:
	/**
	* Removes the association of tags from an Amazon DynamoDB resource. You can call UntagResource up to five times per second, per account.
	*
	* For an overview on tagging DynamoDB resources, see Tagging for DynamoDB in the Amazon DynamoDB Developer Guide.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUntagResource* UntagResource(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUntagResourceRequestDynamoDB& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUntagResourceRequestDynamoDB m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateContinuousBackups
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateContinuousBackupsCallback, bool, bSuccess, const FUpdateContinuousBackupsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateContinuousBackups : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateContinuousBackupsCallback Callback;
public:
	/**
	* UpdateContinuousBackups enables or disables point in time recovery for the specified table. A successful UpdateContinuousBackups call returns the current ContinuousBackupsDescription. Continuous backups are ENABLED on all tables at table creation. If point in time recovery is enabled, PointInTimeRecoveryStatus will be set to ENABLED.
	*
	* Once continuous backups and point in time recovery are enabled, you can restore to any point in time within EarliestRestorableDateTime and LatestRestorableDateTime.
	* LatestRestorableDateTime is typically 5 minutes before the current time. You can restore your table to any point in time during the last 35 days.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateContinuousBackups* UpdateContinuousBackups(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateContinuousBackupsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateContinuousBackupsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateContributorInsights
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateContributorInsightsCallback, bool, bSuccess, const FUpdateContributorInsightsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateContributorInsights : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateContributorInsightsCallback Callback;
public:
	/**
	* Updates the status for contributor insights for a specific table or index.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateContributorInsights* UpdateContributorInsights(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateContributorInsightsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateContributorInsightsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateGlobalTable
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateGlobalTableCallback, bool, bSuccess, const FUpdateGlobalTableResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateGlobalTable : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateGlobalTableCallback Callback;
public:
	/**
	* Adds or removes replicas in the specified global table.
	* The global table must already exist to be able to use this operation. Any replica to be added must be empty, have the same name as the global table, have the same key schema, have DynamoDB Streams enabled, and have the same provisioned and maximum write capacity units.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateGlobalTable* UpdateGlobalTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateGlobalTableRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateGlobalTableRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateGlobalTableSettings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateGlobalTableSettingsCallback, bool, bSuccess, const FUpdateGlobalTableSettingsResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateGlobalTableSettings : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateGlobalTableSettingsCallback Callback;
public:
	/**
	* Updates settings for a global table.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateGlobalTableSettings* UpdateGlobalTableSettings(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateGlobalTableSettingsRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateGlobalTableSettingsRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateItem
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateItemCallback, bool, bSuccess, const FUpdateItemResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateItem : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateItemCallback Callback;
public:
	/**
	* Edits an existing item's attributes, or adds a new item to the table if it does not already exist. You can put, delete, or add attribute values. You can also perform a conditional update on an existing item (insert a new attribute name-value pair if it doesn't exist, or replace an existing name-value pair if it has certain expected attribute values).
	*
	* You can also return the item's attribute values in the same UpdateItem operation using the ReturnValues parameter.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateItem* UpdateItem(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateItemRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateItemRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateTable
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateTableCallback, bool, bSuccess, const FUpdateTableResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateTable : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateTableCallback Callback;
public:
	/**
	* Modifies the provisioned throughput settings, global secondary indexes, or DynamoDB Streams settings for a given table.
	*
	* You can only perform one of the following operations at once:
	* Modify the provisioned throughput settings of the table.
	* Enable or disable DynamoDB Streams on the table.
	* Remove a global secondary index from the table.
	* Create a new global secondary index on the table. After the index begins backfilling, you can use UpdateTable to perform other operations.
	* UpdateTable is an asynchronous operation; while it is executing, the table status changes from ACTIVE to UPDATING. While it is UPDATING, you cannot issue another UpdateTable request. When the table returns to the ACTIVE state, the UpdateTable operation is complete.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateTable* UpdateTable(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateTableRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateTableRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateTableReplicaAutoScaling
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateTableReplicaAutoScalingCallback, bool, bSuccess, const FUpdateTableReplicaAutoScalingResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateTableReplicaAutoScaling : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateTableReplicaAutoScalingCallback Callback;
public:
	/**
	* Updates auto scaling settings on your global tables at once.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateTableReplicaAutoScaling* UpdateTableReplicaAutoScaling(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateTableReplicaAutoScalingRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateTableReplicaAutoScalingRequest m_RequestData;
public:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UDynamoUpdateTimeToLive
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateTimeToLiveCallback, bool, bSuccess, const FUpdateTimeToLiveResult&, result, EDynamoDBErrors, errorType, const FString&, errorMessage);

UCLASS()
class UDynamoUpdateTimeToLive : public UDynamoDBAsync
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateTimeToLiveCallback Callback;
public:
	/**
	* The UpdateTimeToLive method enables or disables Time to Live (TTL) for the specified table. A successful UpdateTimeToLive call returns the current TimeToLiveSpecification. It can take up to one hour for the change to fully process. Any additional UpdateTimeToLive calls for the same table during this one hour duration result in a ValidationException.
	*
	* TTL compares the current time in epoch time format to the time stored in the TTL attribute of an item. If the epoch time value stored in the attribute is less than the current time, the item is marked as expired and subsequently deleted.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "AwsCore|DynamoDB"))
	static UDynamoUpdateTimeToLive* UpdateTimeToLive(UObject* worldContextObject, UDynamoDBClientObject* dynamoDBObject, const FUpdateTimeToLiveRequest& request);
protected:
	UPROPERTY()
	UDynamoDBClientObject* m_DynamoDBObject;
	FUpdateTimeToLiveRequest m_RequestData;
public:
	virtual void Activate() override;
};

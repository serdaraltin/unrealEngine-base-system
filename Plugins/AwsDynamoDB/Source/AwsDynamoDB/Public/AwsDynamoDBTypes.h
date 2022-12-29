/**
* Copyright (C) 2017-2022 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#if !DISABLE_DYNAMODB
THIRD_PARTY_INCLUDES_START
#include <aws/core/auth/AWSCredentials.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/BatchGetItemRequest.h>
#include <aws/dynamodb/model/LocalSecondaryIndex.h>
#include <aws/dynamodb/model/GlobalSecondaryIndex.h>
#include <aws/dynamodb/model/SSESpecification.h>
#include <aws/dynamodb/model/ExpectedAttributeValue.h>
#include <aws/dynamodb/model/Condition.h>
#include <aws/dynamodb/model/Get.h>
#include <aws/dynamodb/model/TransactGetItem.h>
#include <aws/dynamodb/model/Put.h>
#include <aws/dynamodb/model/ConditionCheck.h>
#include <aws/dynamodb/model/Update.h>
#include <aws/dynamodb/model/Delete.h>
#include <aws/dynamodb/model/TransactWriteItem.h>
#include <aws/dynamodb/model/PointInTimeRecoverySpecification.h>
#include <aws/dynamodb/model/CreateReplicaAction.h>
#include <aws/dynamodb/model/DeleteReplicaAction.h>
#include <aws/dynamodb/model/ReplicaUpdate.h>
#include <aws/dynamodb/model/AutoScalingSettingsUpdate.h>
#include <aws/dynamodb/model/GlobalTableGlobalSecondaryIndexSettingsUpdate.h>
#include <aws/dynamodb/model/ReplicaGlobalSecondaryIndexSettingsUpdate.h>
#include <aws/dynamodb/model/ReplicaSettingsUpdate.h>
#include <aws/dynamodb/model/AttributeValueUpdate.h>
#include <aws/dynamodb/model/GlobalSecondaryIndexUpdate.h>
#include <aws/dynamodb/model/UpdateGlobalSecondaryIndexAction.h>
#include <aws/dynamodb/model/DeleteGlobalSecondaryIndexAction.h>
#include <aws/dynamodb/model/UpdateTableRequest.h>
#include <aws/dynamodb/model/GlobalSecondaryIndexAutoScalingUpdate.h>
#include <aws/dynamodb/model/ReplicaAutoScalingUpdate.h>
#include <aws/dynamodb/model/ReplicaGlobalSecondaryIndexAutoScalingUpdate.h>
#include <aws/dynamodb/model/TimeToLiveSpecification.h>
THIRD_PARTY_INCLUDES_END
#endif
#include "AwsDynamoDBTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAwsDynamoDB, Log, All);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

UENUM(BlueprintType)
enum class EDynamoDBErrors : uint8
{
	//From Core//
	//////////////////////////////////////////////////////////////////////////////////////////
	INCOMPLETE_SIGNATURE = 0,
	INTERNAL_FAILURE = 1,
	INVALID_ACTION = 2,
	INVALID_CLIENT_TOKEN_ID = 3,
	INVALID_PARAMETER_COMBINATION = 4,
	INVALID_QUERY_PARAMETER = 5,
	INVALID_PARAMETER_VALUE = 6,
	MISSING_ACTION = 7,
	// SDK should never allow
	MISSING_AUTHENTICATION_TOKEN = 8,
	// SDK should never allow
	MISSING_PARAMETER = 9,
	// SDK should never allow
	OPT_IN_REQUIRED = 10,
	REQUEST_EXPIRED = 11,
	SERVICE_UNAVAILABLE = 12,
	THROTTLING = 13,
	VALIDATION = 14,
	ACCESS_DENIED = 15,
	RESOURCE_NOT_FOUND = 16,
	UNRECOGNIZED_CLIENT = 17,
	MALFORMED_QUERY_STRING = 18,
	SLOW_DOWN = 19,
	REQUEST_TIME_TOO_SKEWED = 20,
	INVALID_SIGNATURE = 21,
	SIGNATURE_DOES_NOT_MATCH = 22,
	INVALID_ACCESS_KEY_ID = 23,
	REQUEST_TIMEOUT = 24,
	NETWORK_CONNECTION = 99,

	UNKNOWN = 100,
	///////////////////////////////////////////////////////////////////////////////////////////

#if !DISABLE_DYNAMODB
	BACKUP_IN_USE = static_cast<int>(Aws::Client::CoreErrors::SERVICE_EXTENSION_START_RANGE) + 1,
#endif
	BACKUP_NOT_FOUND,
	CONDITIONAL_CHECK_FAILED,
	CONTINUOUS_BACKUPS_UNAVAILABLE,
	GLOBAL_TABLE_ALREADY_EXISTS,
	GLOBAL_TABLE_NOT_FOUND,
	IDEMPOTENT_PARAMETER_MISMATCH,
	INDEX_NOT_FOUND,
	INVALID_RESTORE_TIME,
	ITEM_COLLECTION_SIZE_LIMIT_EXCEEDED,
	LIMIT_EXCEEDED,
	POINT_IN_TIME_RECOVERY_UNAVAILABLE,
	PROVISIONED_THROUGHPUT_EXCEEDED,
	REPLICA_ALREADY_EXISTS,
	REPLICA_NOT_FOUND,
	REQUEST_LIMIT_EXCEEDED,
	RESOURCE_IN_USE,
	TABLE_ALREADY_EXISTS,
	TABLE_IN_USE,
	TABLE_NOT_FOUND,
	TRANSACTION_CANCELED,
	TRANSACTION_CONFLICT,
	TRANSACTION_IN_PROGRESS
};

UENUM(BlueprintType)
enum class EValueType : uint8
{
	STRING,
	NUMBER,
	STRING_SET,
	NUMBER_SET,
	BYTEBUFFER_SET,
	ATTRIBUTE_MAP,
	ATTRIBUTE_LIST,
	BOOL
};

UENUM(BlueprintType)
enum class EDynamoDBScheme : uint8
{
	HTTP,
	HTTPS
};

UENUM(BlueprintType)
enum class EDynamoDBFollowRedirectsPolicy : uint8
{
	DEFAULT,
	ALWAYS,
	NEVER
};

UENUM(BlueprintType)
enum class EScalarAttributeType : uint8
{
	NOT_SET,
	S,
	N,
	B
};

UENUM(BlueprintType)
enum class EKeyType : uint8
{
	NOT_SET,
	HASH,
	RANGE
};

UENUM(BlueprintType)
enum class EProjectionType : uint8
{
	NOT_SET,
	ALL,
	KEYS_ONLY,
	INCLUDE
};

UENUM(BlueprintType)
enum class EBillingMode : uint8
{
	NOT_SET,
	PROVISIONED,
	PAY_PER_REQUEST
};

UENUM(BlueprintType)
enum class EStreamViewType : uint8
{
	NOT_SET,
	NEW_IMAGE,
	OLD_IMAGE,
	NEW_AND_OLD_IMAGES,
	KEYS_ONLY
};

UENUM(BlueprintType)
enum class ESSEType : uint8
{
	NOT_SET,
	AES256,
	KMS
};

UENUM(BlueprintType)
enum class EComparisonOperator : uint8
{
	NOT_SET,
	EQ,
	NE,
	INN,
	LE,
	LT,
	GE,
	GT,
	BETWEEN,
	NOT_NULL,
	NULL_,
	CONTAINS,
	NOT_CONTAINS,
	BEGINS_WITH
};

UENUM(BlueprintType)
enum class EConditionalOperator : uint8
{
	NOT_SET,
	AND,
	OR
};

UENUM(BlueprintType)
enum class EReturnValue : uint8
{
	NOT_SET,
	NONE,
	ALL_OLD,
	UPDATED_OLD,
	ALL_NEW,
	UPDATED_NEW
};

UENUM(BlueprintType)
enum class EReturnConsumedCapacity : uint8
{
	NOT_SET,
	INDEXES,
	TOTAL,
	NONE
};

UENUM(BlueprintType)
enum class EReturnItemCollectionMetrics : uint8
{
	NOT_SET,
	SIZE,
	NONE
};

UENUM(BlueprintType)
enum class EBackupTypeFilter : uint8
{
	NOT_SET,
	USER,
	SYSTEM,
	AWS_BACKUP,
	ALL
};

UENUM(BlueprintType)
enum class ESelect : uint8
{
	NOT_SET,
	ALL_ATTRIBUTES,
	ALL_PROJECTED_ATTRIBUTES,
	SPECIFIC_ATTRIBUTES,
	COUNT
};

UENUM(BlueprintType)
enum class EReturnValuesOnConditionCheckFailure : uint8
{
	NOT_SET,
	ALL_OLD,
	NONE
};

UENUM(BlueprintType)
enum class EContributorInsightsAction : uint8
{
	NOT_SET,
	ENABLE,
	DISABLE
};

UENUM(BlueprintType)
enum class EAttributeAction : uint8
{
	NOT_SET,
	ADD,
	PUT,
	DELETE_
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CREDENTIALS AND CONFIG
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FAwsDynamoDBCredentials
{
	GENERATED_BODY()
public:
	FAwsDynamoDBCredentials()
		: AccessKeyId(FString())
		  , SecretKey(FString())
		  , SessionToken(FString())
		  , Expiration(0)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::Auth::AWSCredentials() const
	{
		Aws::Auth::AWSCredentials Data = {};
		if (AccessKeyId.IsEmpty() == false)
			Data.SetAWSAccessKeyId(TCHAR_TO_UTF8(*AccessKeyId));
		if (SecretKey.IsEmpty() == false)
			Data.SetAWSSecretKey(TCHAR_TO_UTF8(*SecretKey));
		if (SessionToken.IsEmpty() == false)
			Data.SetSessionToken(TCHAR_TO_UTF8(*SessionToken));
		if (Expiration.GetTicks() > 0)
			Data.SetExpiration(Aws::Utils::DateTime(TCHAR_TO_UTF8(*Expiration.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));
		return Data;
	}

	bool operator==(const FAwsDynamoDBCredentials& other)
	{
		return other.AccessKeyId == AccessKeyId && other.SecretKey == SecretKey;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString AccessKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString SecretKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString SessionToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FDateTime Expiration;
};

USTRUCT(BlueprintType)
struct FAwsDynamoDBClientConfiguration
{
	GENERATED_BODY()
public:
	FAwsDynamoDBClientConfiguration()
		: Sceheme(EDynamoDBScheme::HTTPS)
		  , Region("eu-central-1")
		  , bUseDualStack(false)
		  , MaxConnections(25)
		  , HTTP_RequestTimeoutMs(0)
		  , RequestTimeoutMs(3000)
		  , ConnectTimeoutMs(1000)
		  , bEnableTcpKeepAlive(true)
		  , TcpKeepAliveIntervalMs(30)
		  , LowSpeedLimit(1)
		  , ProxyScheme(EDynamoDBScheme::HTTP)
		  , ProxyPort(0)
		  , bVerifySSL(false)
		  , FollowRedirects(EDynamoDBFollowRedirectsPolicy::DEFAULT)
		  , bDisableExpectHeader(false)
		  , bEnableClockSkewAdjustment(true)
		  , bEnableHostPrefixInjection(true)
		  , bEnableEndpointDiscovery(false)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::Client::ClientConfiguration() const
	{
		Aws::Client::ClientConfiguration Data = {};
		if (!UserAgent.IsEmpty())
			Data.userAgent = TCHAR_TO_UTF8(*UserAgent);
		Data.scheme = static_cast<Aws::Http::Scheme>(Sceheme);
		if (Region.IsEmpty() == false)
			Data.region = TCHAR_TO_UTF8(*Region);
		Data.useDualStack = bUseDualStack;
		Data.maxConnections = MaxConnections;
		Data.httpRequestTimeoutMs = HTTP_RequestTimeoutMs;
		Data.requestTimeoutMs = RequestTimeoutMs;
		Data.connectTimeoutMs = ConnectTimeoutMs;
		Data.enableTcpKeepAlive = bEnableTcpKeepAlive;
		Data.tcpKeepAliveIntervalMs = TcpKeepAliveIntervalMs;
		Data.lowSpeedLimit = LowSpeedLimit;
		if (EndPointOverride.IsEmpty() == false)
			Data.endpointOverride = TCHAR_TO_UTF8(*EndPointOverride);
		Data.proxyScheme = static_cast<Aws::Http::Scheme>(ProxyScheme);
		if (ProxyHost.IsEmpty() == false)
			Data.proxyHost = TCHAR_TO_UTF8(*ProxyHost);
		Data.proxyPort = ProxyPort;
		if (ProxyUsername.IsEmpty() == false)
			Data.proxyUserName = TCHAR_TO_UTF8(*ProxyUsername);
		if (ProxyPassword.IsEmpty() == false)
			Data.proxyPassword = TCHAR_TO_UTF8(*ProxyPassword);
		if (ProxySSL_CertPath.IsEmpty() == false)
			Data.proxySSLCertPath = TCHAR_TO_UTF8(*ProxySSL_CertPath);
		if (ProxySSL_CertType.IsEmpty() == false)
			Data.proxySSLCertType = TCHAR_TO_UTF8(*ProxySSL_CertType);
		if (ProxySSL_KeyPath.IsEmpty() == false)
			Data.proxySSLKeyPath = TCHAR_TO_UTF8(*ProxySSL_KeyPath);
		if (ProxySSL_KeyType.IsEmpty() == false)
			Data.proxySSLKeyType = TCHAR_TO_UTF8(*ProxySSL_KeyType);
		if (ProxySSL_KeyPassword.IsEmpty() == false)
			Data.proxySSLKeyPassword = TCHAR_TO_UTF8(*ProxySSL_KeyPassword);
		Data.verifySSL = bVerifySSL;
		if (CaPath.IsEmpty() == false)
			Data.caPath = TCHAR_TO_UTF8(*CaPath);
		if (CaFile.IsEmpty() == false)
			Data.caFile = TCHAR_TO_UTF8(*CaFile);

		Data.followRedirects = static_cast<Aws::Client::FollowRedirectsPolicy>(FollowRedirects);
		Data.disableExpectHeader = bDisableExpectHeader;
		Data.enableClockSkewAdjustment = bEnableClockSkewAdjustment;
		Data.enableHostPrefixInjection = bEnableHostPrefixInjection;
		Data.enableEndpointDiscovery = bEnableEndpointDiscovery;

		if (ProfileName.IsEmpty() == false)
			Data.profileName = TCHAR_TO_UTF8(*ProfileName);

		return Data;
	}

	bool operator==(const FAwsDynamoDBClientConfiguration& other)
	{
		return other.Region == Region;
	}
#endif
public:
	// User Agent string user for http calls. This is filled in for you in the constructor. Don't override this unless you have a really good reason.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString UserAgent;
	// Http scheme to use. E.g. Http or Https. Default HTTPS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	EDynamoDBScheme Sceheme;
	// AWS Region to use in signing requests. Default eu-west-2
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString Region;
	// Use dual stack endpoint32 in the endpoint32 calculation. It is your responsibility to verify that the service supports ipv6 in the region you select.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	bool bUseDualStack;
	// Max concurrent tcp connections for a single http client to use. Default 25.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	int32 MaxConnections;
	/**
	 * This is currently only applicable for Curl to set the http request level timeout, including possible dns lookup time, connection establish time, ssl handshake time and actual data transmission time.
	 * the corresponding Curl option is CURLOPT_TIMEOUT_MS
	 * defaults to 0, no http request level timeout.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	int32 HTTP_RequestTimeoutMs;
	/**
	 * Socket read timeouts for HTTP clients on Windows. Default 3000 ms. This should be more than adequate for most services. However, if you are transfering large amounts of data
	 * or are worried about higher latencies, you should set to something that makes more sense for your use case.
	 * For Curl, it's the low speed time, which contains the time in number milliseconds that transfer speed should be below "lowSpeedLimit" for the library to consider it too slow and abort.
	 * Note that for Curl this config is converted to seconds by rounding down to the nearest whole second except when the value is greater than 0 and less than 1000. In this case it is set to one second. When it's 0, low speed limit check will be disabled.
	 * Note that for Windows when this config is 0, the behavior is not specified by Windows.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	int32 RequestTimeoutMs;
	/**
	 * Socket connect timeout. Default 1000 ms. Unless you are very far away from your the data center you are talking to. 1000ms is more than sufficient.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	int32 ConnectTimeoutMs;
	/**
	 * Enable TCP keep-alive. Default true;
	 * No-op for WinHTTP, WinINet and IXMLHTTPRequest2 client.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	bool bEnableTcpKeepAlive;
	/**
	 * Interval to send a keep-alive packet over the connection. Default 30 seconds. Minimum 15 seconds.
	 * WinHTTP & libcurl support this option. Note that for Curl, this value will be rounded to an integer with second granularity.
	 * No-op for WinINet and IXMLHTTPRequest2 client.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	int32 TcpKeepAliveIntervalMs;
	/**
	 * Average transfer speed in bytes per second that the transfer should be below during the request timeout interval for it to be considered too slow and abort.
	 * Default 1 byte/second. Only for CURL client currently.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	int32 LowSpeedLimit;
	/**
	 * Override the http endpoint32 used to talk to a service.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString EndPointOverride;
	/**
	 * If you have users going through a proxy, set the proxy scheme here. Default HTTP
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	EDynamoDBScheme ProxyScheme;
	/**
	 * If you have users going through a proxy, set the host here.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxyHost;
	/**
	 * If you have users going through a proxy, set the port here.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	int32 ProxyPort;
	/**
	 * If you have users going through a proxy, set the username here.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxyUsername;
	/**
	* If you have users going through a proxy, set the password here.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxyPassword;
	/**
	* SSL Certificate file to use for connecting to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_SSLCERT in libcurl. Example: client.pem
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxySSL_CertPath;
	/**
	* Type of proxy client SSL certificate.
	* Used to set CURLOPT_PROXY_SSLCERTTYPE in libcurl. Example: PEM
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxySSL_CertType;
	/**
	* Private key file to use for connecting to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_SSLKEY in libcurl. Example: key.pem
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxySSL_KeyPath;
	/**
	* Type of private key file used to connect to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_SSLKEYTYPE in libcurl. Example: PEM
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxySSL_KeyType;
	/**
	* Passphrase to the private key file used to connect to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_KEYPASSWD in libcurl. Example: password1
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProxySSL_KeyPassword;
	/**
	 * If you need to test and want to get around TLS validation errors, do that here.
	 * you probably shouldn't use this flag in a production scenario.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	bool bVerifySSL;
	/**
	 * If your Certificate Authority path is different from the default, you can tell
	 * clients that aren't using the default trust store where to find your CA trust store.
	 * If you are on windows or apple, you likely don't want this.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString CaPath;
	/**
	 * If you certificate file is different from the default, you can tell clients that
	 * aren't using the default trust store where to find your ca file.
	 * If you are on windows or apple, you likely don't want this.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString CaFile;
	/**
	 * Sets the behavior how http stack handles 30x redirect codes.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	EDynamoDBFollowRedirectsPolicy FollowRedirects;
	/**
	 * Only works for Curl http client.
	 * Curl will by default add "Expect: 100-Continue" header in a Http request so as to avoid sending http
	 * payload to wire if server respond error immediately after receiving the header.
	 * Set this option to true will tell Curl to send http request header and body together.
	 * This can save one round-trip time and especially useful when the payload is small and network latency is more important.
	 * But be careful when Http request has large payload such S3 PutObject. You don't want to spend long time sending a large payload just getting a error response for server.
	 * The default value will be false.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	bool bDisableExpectHeader;
	/**
	 * If set to true clock skew will be adjusted after each http attempt, default to true.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	bool bEnableClockSkewAdjustment;

	/**
	 * Enable host prefix injection.
	 * For services whose endpoint32 is injectable. e.g. servicediscovery, you can modify the http host's prefix so as to add "data-" prefix for DiscoverInstances request.
	 * Default to true, enabled. You can disable it for testing purpose.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	bool bEnableHostPrefixInjection;

	/**
	 * Enable endpoint32 discovery
	 * For some services to dynamically set up their endpoints for different requests.
	 * Defaults to false, it's an opt-in feature.
	 * If disabled, regional or overriden endpoint32 will be used instead.
	 * If a request requires endpoint32 discovery but you disabled it. The request will never succeed.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	bool bEnableEndpointDiscovery;

	/**
	 * profileName in config file that will be used by this object to reslove more configurations.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|Lambda")
	FString ProfileName;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FAttributeValue
{
	GENERATED_BODY()
public:
	FAttributeValue()
		: Type(EValueType::BOOL)
	{
	}
#if !DISABLE_DYNAMODB
	FAttributeValue(const Aws::DynamoDB::Model::AttributeValue& data)
	{
		switch (data.GetType())
		{
		case Aws::DynamoDB::Model::ValueType::BOOL:
			Value = FString::Printf(TEXT("%d"), data.GetBool());
			Type = EValueType::BOOL;
			break;
		case Aws::DynamoDB::Model::ValueType::NUMBER:
			Value = LexToString(FCString::Atoi(UTF8_TO_TCHAR(data.GetN().c_str())));
			Type = EValueType::NUMBER;
			break;
		case Aws::DynamoDB::Model::ValueType::STRING:
			Value = UTF8_TO_TCHAR(data.GetS().c_str());
			Type = EValueType::STRING;
			break;
		case Aws::DynamoDB::Model::ValueType::NUMBER_SET:
			Value = UTF8_TO_TCHAR(data.SerializeAttribute().c_str());
			Type = EValueType::NUMBER_SET;
			break;
		case Aws::DynamoDB::Model::ValueType::STRING_SET:
			Value = UTF8_TO_TCHAR(data.SerializeAttribute().c_str());
			Type = EValueType::STRING_SET;
			break;
		case Aws::DynamoDB::Model::ValueType::ATTRIBUTE_MAP:
			Value = UTF8_TO_TCHAR(data.SerializeAttribute().c_str());
			Type = EValueType::ATTRIBUTE_MAP;
			break;
		case Aws::DynamoDB::Model::ValueType::ATTRIBUTE_LIST:
			Value = UTF8_TO_TCHAR(data.SerializeAttribute().c_str());
			Type = EValueType::ATTRIBUTE_LIST;
			break;
		default: ;
		}
	}

public:
	operator Aws::DynamoDB::Model::AttributeValue() const
	{
		Aws::DynamoDB::Model::AttributeValue Data = {};
		Aws::Utils::Json::JsonValue JsonValue = Aws::String(TCHAR_TO_UTF8(*Value));
		switch (Type)
		{
		case EValueType::STRING:
			Data.SetS(TCHAR_TO_UTF8(*Value));
			break;
		case EValueType::NUMBER:
			Data.SetN(FCString::Atod(*Value));
			break;
		case EValueType::BOOL:
			Data.SetBool(FCString::Atoi(*Value) > 0);
			break;
		case EValueType::NUMBER_SET:
			Data = Aws::DynamoDB::Model::AttributeValue(JsonValue);
			break;
		case EValueType::STRING_SET:
			Data = Aws::DynamoDB::Model::AttributeValue(JsonValue);
			break;
		case EValueType::ATTRIBUTE_MAP:
			Data = Aws::DynamoDB::Model::AttributeValue(JsonValue);
			break;
		case EValueType::ATTRIBUTE_LIST:
			Data = Aws::DynamoDB::Model::AttributeValue(JsonValue);
			break;
		default: ;
		}
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString Value;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EValueType Type;
};

USTRUCT(BlueprintType)
struct FKeysMap
{
	GENERATED_BODY()
public:
	FKeysMap() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Map;
};

USTRUCT(BlueprintType)
struct FKeysAndAttributes
{
	GENERATED_BODY()
public:
	FKeysAndAttributes()
		: bConsistentRead(false)
	{
	}
#if !DISABLE_DYNAMODB
	FKeysAndAttributes(const Aws::DynamoDB::Model::KeysAndAttributes& data)
		: bConsistentRead(data.GetConsistentRead())
		  , ProjectionExpression(UTF8_TO_TCHAR(data.GetProjectionExpression().c_str()))
	{
		for (auto& Element : data.GetAttributesToGet())
			AttributesToGet.Add(UTF8_TO_TCHAR(Element.c_str()));

		for (auto& Element : data.GetExpressionAttributeNames())
			ExpressionAttributeNames.Add(UTF8_TO_TCHAR(Element.first.c_str()), UTF8_TO_TCHAR(Element.second.c_str()));

		for (auto& Element : data.GetKeys())
		{
			FKeysMap KeysMap;

			for (auto& MapElement : Element)
			{
				KeysMap.Map.Add(UTF8_TO_TCHAR(MapElement.first.c_str()), MapElement.second);
			}

			Keys.Add(KeysMap);
		}
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> AttributesToGet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bConsistentRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProjectionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeysMap> Keys;
};

USTRUCT(BlueprintType)
struct FBatchGetItemRequest
{
	GENERATED_BODY()
public:
	FBatchGetItemRequest()
		: ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FBatchGetItemRequest(const Aws::DynamoDB::Model::BatchGetItemRequest& data)
		: ReturnConsumedCapacity(static_cast<EReturnConsumedCapacity>(data.GetReturnConsumedCapacity()))
	{
		for (auto& Element : data.GetRequestItems())
			RequestItems.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FKeysAndAttributes> RequestItems;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
};

USTRUCT(BlueprintType)
struct FPutRequest
{
	GENERATED_BODY()
public:
	FPutRequest() = default;
#if !DISABLE_DYNAMODB
	FPutRequest(const Aws::DynamoDB::Model::PutRequest& data)
	{
		for (auto& Element : data.GetItem())
		{
			Item.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
		}
	}

public:
	operator Aws::DynamoDB::Model::PutRequest() const
	{
		Aws::DynamoDB::Model::PutRequest Data = {};
		for (auto& Element : Item)
			Data.AddItem(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Item;
};

USTRUCT(BlueprintType)
struct FDeleteRequest
{
	GENERATED_BODY()
public:
	FDeleteRequest() = default;
#if !DISABLE_DYNAMODB
	FDeleteRequest(const Aws::DynamoDB::Model::DeleteRequest& data)
	{
		for (auto& Element : data.GetKey())
		{
			Key.Add(UTF8_TO_TCHAR(Element.first.c_str()), Element.second);
		}
	}

public:
	operator Aws::DynamoDB::Model::DeleteRequest() const
	{
		Aws::DynamoDB::Model::DeleteRequest Data = {};
		for (auto& Element : Key)
			Data.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
};

USTRUCT(BlueprintType)
struct FWriteRequest
{
	GENERATED_BODY()
public:
	FWriteRequest() = default;
#if !DISABLE_DYNAMODB
	FWriteRequest(const Aws::DynamoDB::Model::WriteRequest& data)
		: PutRequest(data.GetPutRequest())
		  , DeleteRequest(data.GetDeleteRequest())
	{
	}

public:
	operator Aws::DynamoDB::Model::WriteRequest() const
	{
		Aws::DynamoDB::Model::WriteRequest Data = {};
		if (PutRequest.Item.Num() > 0)
			Data.SetPutRequest(PutRequest);
		if (DeleteRequest.Key.Num() > 0)
			Data.SetDeleteRequest(DeleteRequest);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FPutRequest PutRequest;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDeleteRequest DeleteRequest;
};

USTRUCT(BlueprintType)
struct FWriteRequestArray
{
	GENERATED_BODY()
public:
	FWriteRequestArray() = default;

	FWriteRequestArray(TArray<FWriteRequest> InRequests)
		: Requests(InRequests)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FWriteRequest> Requests;
};

USTRUCT(BlueprintType)
struct FBatchWriteItemRequest
{
	GENERATED_BODY()
public:
	FBatchWriteItemRequest()
		: ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
		  , ReturnItemCollectionMetrics(EReturnItemCollectionMetrics::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FWriteRequestArray> RequestItems;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnItemCollectionMetrics ReturnItemCollectionMetrics;
};

USTRUCT(BlueprintType)
struct FCreateBackupRequest
{
	GENERATED_BODY()
public:
	FCreateBackupRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupName;
};

USTRUCT(BlueprintType)
struct FReplica
{
	GENERATED_BODY()
public:
	FReplica() = default;
#if !DISABLE_DYNAMODB
	FReplica(const Aws::DynamoDB::Model::Replica& data)
		: RegionName(UTF8_TO_TCHAR(data.GetRegionName().c_str()))
	{
	}

public:
	operator Aws::DynamoDB::Model::Replica() const
	{
		Aws::DynamoDB::Model::Replica Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
};

USTRUCT(BlueprintType)
struct FCreateGlobalTableRequest
{
	GENERATED_BODY()
public:
	FCreateGlobalTableRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplica> ReplicationGroup;
};

USTRUCT(BlueprintType)
struct FAttributeDefinition
{
	GENERATED_BODY()
public:
	FAttributeDefinition()
		: AttributeType(EScalarAttributeType::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FAttributeDefinition(const Aws::DynamoDB::Model::AttributeDefinition& data)
		: AttributeName(UTF8_TO_TCHAR(data.GetAttributeName().c_str()))
		  , AttributeType(static_cast<EScalarAttributeType>(data.GetAttributeType()))
	{
	}

public:
	operator Aws::DynamoDB::Model::AttributeDefinition() const
	{
		Aws::DynamoDB::Model::AttributeDefinition Data = {};
		if (AttributeName.IsEmpty() == false)
			Data.SetAttributeName(TCHAR_TO_UTF8(*AttributeName));
		if (AttributeType != EScalarAttributeType::NOT_SET)
			Data.SetAttributeType(static_cast<Aws::DynamoDB::Model::ScalarAttributeType>(AttributeType));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString AttributeName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EScalarAttributeType AttributeType;
};

USTRUCT(BlueprintType)
struct FKeySchemaElement
{
	GENERATED_BODY()
public:
	FKeySchemaElement()
		: KeyType(EKeyType::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FKeySchemaElement(const Aws::DynamoDB::Model::KeySchemaElement& data)
		: AttributeName(UTF8_TO_TCHAR(data.GetAttributeName().c_str()))
		  , KeyType(static_cast<EKeyType>(data.GetKeyType()))
	{
	}

public:
	operator Aws::DynamoDB::Model::KeySchemaElement() const
	{
		Aws::DynamoDB::Model::KeySchemaElement Data = {};
		if (AttributeName.IsEmpty() == false)
			Data.SetAttributeName(TCHAR_TO_UTF8(*AttributeName));
		if (KeyType != EKeyType::NOT_SET)
			Data.SetKeyType(static_cast<Aws::DynamoDB::Model::KeyType>(KeyType));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString AttributeName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EKeyType KeyType;
};

USTRUCT(BlueprintType)
struct FProjection
{
	GENERATED_BODY()
public:
	FProjection()
		: ProjectionType(EProjectionType::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	FProjection(const Aws::DynamoDB::Model::Projection& data)
		: ProjectionType(static_cast<EProjectionType>(data.GetProjectionType()))
	{
		for (auto& Element : data.GetNonKeyAttributes())
			NonKeyAttributes.Add(UTF8_TO_TCHAR(Element.c_str()));
	}

public:
	operator Aws::DynamoDB::Model::Projection() const
	{
		Aws::DynamoDB::Model::Projection Data = {};
		if (ProjectionType != EProjectionType::NOT_SET)
			Data.SetProjectionType(static_cast<Aws::DynamoDB::Model::ProjectionType>(ProjectionType));

		for (auto& Element : NonKeyAttributes)
			Data.AddNonKeyAttributes(TCHAR_TO_UTF8(*Element));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EProjectionType ProjectionType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> NonKeyAttributes;
};

USTRUCT(BlueprintType)
struct FLocalSecondaryIndex
{
	GENERATED_BODY()
public:
	FLocalSecondaryIndex() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::LocalSecondaryIndex() const
	{
		Aws::DynamoDB::Model::LocalSecondaryIndex Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		for (auto& Element : KeySchema)
			Data.AddKeySchema(Element);
		Data.SetProjection(Projection);
		return Data;
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
struct FProvisionedThroughput
{
	GENERATED_BODY()
public:
	FProvisionedThroughput() = default;
#if !DISABLE_DYNAMODB
	FProvisionedThroughput(const Aws::DynamoDB::Model::ProvisionedThroughput& data)
		: ReadCapacityUnits(LexToString(data.GetReadCapacityUnits()))
		  , WriteCapacityUnits(LexToString(data.GetWriteCapacityUnits()))
	{
	}

public:
	operator Aws::DynamoDB::Model::ProvisionedThroughput() const
	{
		Aws::DynamoDB::Model::ProvisionedThroughput Data = {};
		if (ReadCapacityUnits.IsEmpty() == false)
			Data.SetReadCapacityUnits(FCString::Atod(*ReadCapacityUnits));
		if (WriteCapacityUnits.IsEmpty() == false)
			Data.SetWriteCapacityUnits(FCString::Atod(*WriteCapacityUnits));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString WriteCapacityUnits;
};

USTRUCT(BlueprintType)
struct FGlobalSecondaryIndex
{
	GENERATED_BODY()
public:
	FGlobalSecondaryIndex() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::GlobalSecondaryIndex() const
	{
		Aws::DynamoDB::Model::GlobalSecondaryIndex Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));

		for (auto& Element : KeySchema)
			Data.AddKeySchema(Element);

		Data.SetProjection(Projection);
		Data.SetProvisionedThroughput(ProvisionedThroughput);

		return Data;
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
struct FStreamSpecification
{
	GENERATED_BODY()
public:
	FStreamSpecification()
		: bStreamEnabled(false)
		  , StreamViewType(EStreamViewType::NOT_SET)
	{
	}
#if !DISABLE_DYNAMODB
	FStreamSpecification(const Aws::DynamoDB::Model::StreamSpecification& data)
		: bStreamEnabled(data.GetStreamEnabled())
		  , StreamViewType(static_cast<EStreamViewType>(data.GetStreamViewType()))
	{
	}

public:
	operator Aws::DynamoDB::Model::StreamSpecification() const
	{
		Aws::DynamoDB::Model::StreamSpecification Data = {};
		Data.SetStreamEnabled(bStreamEnabled);
		if (StreamViewType != EStreamViewType::NOT_SET)
			Data.SetStreamViewType(static_cast<Aws::DynamoDB::Model::StreamViewType>(StreamViewType));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bStreamEnabled;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EStreamViewType StreamViewType;
};

USTRUCT(BlueprintType)
struct FSSESpecification
{
	GENERATED_BODY()
public:
	FSSESpecification()
		: bEnabled(false)
		  , SSEType(ESSEType::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::SSESpecification() const
	{
		Aws::DynamoDB::Model::SSESpecification Data = {};
		Data.SetEnabled(bEnabled);
		if (SSEType != ESSEType::NOT_SET)
			Data.SetSSEType(static_cast<Aws::DynamoDB::Model::SSEType>(SSEType));
		Data.SetKMSMasterKeyId(TCHAR_TO_UTF8(*KMSMasterKeyId));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bEnabled;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ESSEType SSEType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString KMSMasterKeyId;
};

USTRUCT(BlueprintType)
struct FDynamoDBTag
{
	GENERATED_BODY()
public:
	FDynamoDBTag() = default;
#if !DISABLE_DYNAMODB
	FDynamoDBTag(const Aws::DynamoDB::Model::Tag& data)
		: Key(UTF8_TO_TCHAR(data.GetKey().c_str()))
		  , Value(UTF8_TO_TCHAR(data.GetValue().c_str()))
	{
	}

public:
	operator Aws::DynamoDB::Model::Tag() const
	{
		Aws::DynamoDB::Model::Tag Data = {};
		if (Key.IsEmpty() == false)
			Data.SetKey(TCHAR_TO_UTF8(*Key));
		if (Value.IsEmpty() == false)
			Data.SetValue(TCHAR_TO_UTF8(*Value));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString Value;
};

USTRUCT(BlueprintType)
struct FCreateTableRequest
{
	GENERATED_BODY()
public:
	FCreateTableRequest()
		: BillingMode(EBillingMode::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeDefinition> AttributeDefinitions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FKeySchemaElement> KeySchema;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FLocalSecondaryIndex> LocalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalSecondaryIndex> GlobalSecondaryIndexes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBillingMode BillingMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughput ProvisionedThroughput;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FStreamSpecification StreamSpecification;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSSESpecification SSESpecification;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FDynamoDBTag> Tags;
};

USTRUCT(BlueprintType)
struct FDeleteBackupRequest
{
	GENERATED_BODY()
public:
	FDeleteBackupRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupArn;
};

USTRUCT(BlueprintType)
struct FExpectedAttributeValue
{
	GENERATED_BODY()
public:
	FExpectedAttributeValue()
		: bExists(false)
		  , ComparisonOperator(EComparisonOperator::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ExpectedAttributeValue() const
	{
		Aws::DynamoDB::Model::ExpectedAttributeValue Data = {};
		Data.SetValue(Value);
		Data.SetExists(bExists);
		if (ComparisonOperator != EComparisonOperator::NOT_SET)
			Data.SetComparisonOperator(static_cast<Aws::DynamoDB::Model::ComparisonOperator>(ComparisonOperator));
		for (auto& Element : AttributeValueList)
			Data.AddAttributeValueList(Element);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAttributeValue Value;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bExists;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EComparisonOperator ComparisonOperator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeValue> AttributeValueList;
};

USTRUCT(BlueprintType)
struct FDeleteItemRequest
{
	GENERATED_BODY()
public:
	FDeleteItemRequest()
		: ConditionalOperator(EConditionalOperator::NOT_SET)
		  , ReturnValues(EReturnValue::NOT_SET)
		  , ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
		  , ReturnItemCollectionMetrics(EReturnItemCollectionMetrics::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FExpectedAttributeValue> Expected;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EConditionalOperator ConditionalOperator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnValue ReturnValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnItemCollectionMetrics ReturnItemCollectionMetrics;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
};

USTRUCT(BlueprintType)
struct FDeleteTableRequest
{
	GENERATED_BODY()
public:
	FDeleteTableRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
};

USTRUCT(BlueprintType)
struct FDescribeBackupRequest
{
	GENERATED_BODY()
public:
	FDescribeBackupRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupArn;
};

USTRUCT(BlueprintType)
struct FDescribeContinuousBackupsRequest
{
	GENERATED_BODY()
public:
	FDescribeContinuousBackupsRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
};

USTRUCT(BlueprintType)
struct FDescribeContributorInsightsRequest
{
	GENERATED_BODY()
public:
	FDescribeContributorInsightsRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
};

USTRUCT(BlueprintType)
struct FDescribeEndpointsRequest
{
	GENERATED_BODY()
public:
	FDescribeEndpointsRequest() = default;
};

USTRUCT(BlueprintType)
struct FDescribeGlobalTableRequest
{
	GENERATED_BODY()
public:
	FDescribeGlobalTableRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
};

USTRUCT(BlueprintType)
struct FDescribeGlobalTableSettingsRequest
{
	GENERATED_BODY()
public:
	FDescribeGlobalTableSettingsRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
};

USTRUCT(BlueprintType)
struct FDescribeLimitsRequest
{
	GENERATED_BODY()
public:
	FDescribeLimitsRequest() = default;
};

USTRUCT(BlueprintType)
struct FDescribeTableRequest
{
	GENERATED_BODY()
public:
	FDescribeTableRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
};

USTRUCT(BlueprintType)
struct FDescribeTableReplicaAutoScalingRequest
{
	GENERATED_BODY()
public:
	FDescribeTableReplicaAutoScalingRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
};

USTRUCT(BlueprintType)
struct FDescribeTimeToLiveRequest
{
	GENERATED_BODY()
public:
	FDescribeTimeToLiveRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
};

USTRUCT(BlueprintType)
struct FGetItemRequest
{
	GENERATED_BODY()
public:
	FGetItemRequest()
		: bConsistentRead(false)
		  , ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> AttributesToGet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bConsistentRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProjectionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
};

USTRUCT(BlueprintType)
struct FListBackupsRequest
{
	GENERATED_BODY()
public:
	FListBackupsRequest()
		: Limit(0)
		  , TimeRangeLowerBound(0)
		  , TimeRangeUpperBound(0)
		  , BackupType(EBackupTypeFilter::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Limit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime TimeRangeLowerBound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime TimeRangeUpperBound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ExclusiveStartBackupArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBackupTypeFilter BackupType;
};

USTRUCT(BlueprintType)
struct FListContributorInsightsRequest
{
	GENERATED_BODY()
public:
	FListContributorInsightsRequest()
		: MaxResults(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString NextToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 MaxResults;
};

USTRUCT(BlueprintType)
struct FListGlobalTablesRequest
{
	GENERATED_BODY()
public:
	FListGlobalTablesRequest()
		: Limit(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ExclusiveStartGlobalTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Limit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
};

USTRUCT(BlueprintType)
struct FListTablesRequest
{
	GENERATED_BODY()
public:
	FListTablesRequest()
		: Limit(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ExclusiveStartTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Limit;
};

USTRUCT(BlueprintType)
struct FListTagsOfResourceRequest
{
	GENERATED_BODY()
public:
	FListTagsOfResourceRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ResourceArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString NextToken;
};

USTRUCT(BlueprintType)
struct FPutItemRequest
{
	GENERATED_BODY()
public:
	FPutItemRequest()
		: ReturnValues(EReturnValue::NOT_SET)
		  , ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
		  , ReturnItemCollectionMetrics(EReturnItemCollectionMetrics::NOT_SET)
		  , ConditionalOperator(EConditionalOperator::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Item;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FExpectedAttributeValue> Expected;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnValue ReturnValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnItemCollectionMetrics ReturnItemCollectionMetrics;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EConditionalOperator ConditionalOperator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
};

USTRUCT(BlueprintType)
struct FCondition
{
	GENERATED_BODY()
public:
	FCondition()
		: ComparisonOperator(EComparisonOperator::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::Condition() const
	{
		Aws::DynamoDB::Model::Condition Data = {};
		for (auto& Element : AttributeValueList)
			Data.AddAttributeValueList(Element);
		if (ComparisonOperator != EComparisonOperator::NOT_SET)
			Data.SetComparisonOperator(static_cast<Aws::DynamoDB::Model::ComparisonOperator>(ComparisonOperator));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeValue> AttributeValueList;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EComparisonOperator ComparisonOperator;
};

USTRUCT(BlueprintType)
struct FQueryRequest
{
	GENERATED_BODY()
public:
	FQueryRequest()
		: Select(ESelect::NOT_SET)
		  , Limit(0)
		  , bConsistentRead(false)
		  , ConditionalOperator(EConditionalOperator::NOT_SET)
		  , bScanIndexForward(false)
		  , ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ESelect Select;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> AttributesToGet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Limit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bConsistentRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FCondition> KeyConditions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FCondition> QueryFilter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EConditionalOperator ConditionalOperator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bScanIndexForward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExclusiveStartKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProjectionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString FilterExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString KeyConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
};

USTRUCT(BlueprintType)
struct FRestoreTableFromBackupRequest
{
	GENERATED_BODY()
public:
	FRestoreTableFromBackupRequest()
		: BillingModeOverride(EBillingMode::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TargetTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString BackupArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBillingMode BillingModeOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalSecondaryIndex> GlobalSecondaryIndexOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FLocalSecondaryIndex> LocalSecondaryIndexOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughput ProvisionedThroughputOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSSESpecification SSESpecificationOverride;
};

USTRUCT(BlueprintType)
struct FRestoreTableToPointInTimeRequest
{
	GENERATED_BODY()
public:
	FRestoreTableToPointInTimeRequest()
		: bUseLatestRestorableTime(false)
		  , RestoreDateTime(0)
		  , BillingModeOverride(EBillingMode::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString SourceTableArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString SourceTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TargetTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bUseLatestRestorableTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDateTime RestoreDateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBillingMode BillingModeOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalSecondaryIndex> GlobalSecondaryIndexOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FLocalSecondaryIndex> LocalSecondaryIndexOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughput ProvisionedThroughputOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSSESpecification SSESpecificationOverride;
};

USTRUCT(BlueprintType)
struct FScanRequest
{
	GENERATED_BODY()
public:
	FScanRequest()
		: Limit(0)
		  , Select(ESelect::NOT_SET)
		  , ConditionalOperator(EConditionalOperator::NOT_SET)
		  , ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
		  , TotalSegments(0)
		  , Segment(0)
		  , bConsistentRead(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> AttributesToGet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Limit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	ESelect Select;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FCondition> ScanFilter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EConditionalOperator ConditionalOperator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExclusiveStartKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 TotalSegments;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	int32 Segment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProjectionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString FilterExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bConsistentRead;
};

USTRUCT(BlueprintType)
struct FTagResourceRequestDynamoDB
{
	GENERATED_BODY()
public:
	FTagResourceRequestDynamoDB() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ResourceArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FDynamoDBTag> Tags;
};

USTRUCT(BlueprintType)
struct FDynamoDBGet
{
	GENERATED_BODY()
public:
	FDynamoDBGet() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::Get() const
	{
		Aws::DynamoDB::Model::Get Data = {};
		for (auto& Element : Key)
			Data.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (TableName.IsEmpty() == false)
			Data.SetTableName(TCHAR_TO_UTF8(*TableName));
		if (ProjectionExpression.IsEmpty() == false)
			Data.SetProjectionExpression(TCHAR_TO_UTF8(*ProjectionExpression));
		for (auto& Element : ExpressionAttributeNames)
			Data.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProjectionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
};

USTRUCT(BlueprintType)
struct FTransactGetItem
{
	GENERATED_BODY()
public:
	FTransactGetItem() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::TransactGetItem() const
	{
		Aws::DynamoDB::Model::TransactGetItem Data = {};
		Data.SetGet(Get);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDynamoDBGet Get;
};

USTRUCT(BlueprintType)
struct FTransactGetItemsRequest
{
	GENERATED_BODY()
public:
	FTransactGetItemsRequest()
		: ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FTransactGetItem> TransactItems;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
};

USTRUCT(BlueprintType)
struct FPut
{
	GENERATED_BODY()
public:
	FPut()
		: ReturnValuesOnConditionCheckFailure(EReturnValuesOnConditionCheckFailure::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::Put() const
	{
		Aws::DynamoDB::Model::Put Data = {};
		for (auto& Element : Item)
			Data.AddItem(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (TableName.IsEmpty() == false)
			Data.SetTableName(TCHAR_TO_UTF8(*TableName));
		if (ConditionExpression.IsEmpty() == false)
			Data.SetConditionExpression(TCHAR_TO_UTF8(*ConditionExpression));
		for (auto& Element : ExpressionAttributeNames)
			Data.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));
		for (auto& Element : ExpressionAttributeValues)
			Data.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (ReturnValuesOnConditionCheckFailure != EReturnValuesOnConditionCheckFailure::NOT_SET)
			Data.SetReturnValuesOnConditionCheckFailure(static_cast<Aws::DynamoDB::Model::ReturnValuesOnConditionCheckFailure>(ReturnValuesOnConditionCheckFailure));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Item;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnValuesOnConditionCheckFailure ReturnValuesOnConditionCheckFailure;
};

USTRUCT(BlueprintType)
struct FConditionCheck
{
	GENERATED_BODY()
public:
	FConditionCheck()
		: ReturnValuesOnConditionCheckFailure(EReturnValuesOnConditionCheckFailure::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ConditionCheck() const
	{
		Aws::DynamoDB::Model::ConditionCheck Data = {};
		for (auto& Element : Key)
			Data.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (TableName.IsEmpty() == false)
			Data.SetTableName(TCHAR_TO_UTF8(*TableName));
		if (ConditionExpression.IsEmpty() == false)
			Data.SetConditionExpression(TCHAR_TO_UTF8(*ConditionExpression));
		for (auto& Element : ExpressionAttributeNames)
			Data.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));
		for (auto& Element : ExpressionAttributeValues)
			Data.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (ReturnValuesOnConditionCheckFailure != EReturnValuesOnConditionCheckFailure::NOT_SET)
			Data.SetReturnValuesOnConditionCheckFailure(static_cast<Aws::DynamoDB::Model::ReturnValuesOnConditionCheckFailure>(ReturnValuesOnConditionCheckFailure));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnValuesOnConditionCheckFailure ReturnValuesOnConditionCheckFailure;
};

USTRUCT(BlueprintType)
struct FDeleteDynamoDB
{
	GENERATED_BODY()
public:
	FDeleteDynamoDB()
		: ReturnValuesOnConditionCheckFailure(EReturnValuesOnConditionCheckFailure::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::Delete() const
	{
		Aws::DynamoDB::Model::Delete Data = {};
		for (auto& Element : Key)
			Data.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (TableName.IsEmpty() == false)
			Data.SetTableName(TCHAR_TO_UTF8(*TableName));
		if (ConditionExpression.IsEmpty() == false)
			Data.SetConditionExpression(TCHAR_TO_UTF8(*ConditionExpression));
		for (auto& Element : ExpressionAttributeNames)
			Data.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));
		for (auto& Element : ExpressionAttributeValues)
			Data.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (ReturnValuesOnConditionCheckFailure != EReturnValuesOnConditionCheckFailure::NOT_SET)
			Data.SetReturnValuesOnConditionCheckFailure(static_cast<Aws::DynamoDB::Model::ReturnValuesOnConditionCheckFailure>(ReturnValuesOnConditionCheckFailure));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnValuesOnConditionCheckFailure ReturnValuesOnConditionCheckFailure;
};

USTRUCT(BlueprintType)
struct FUpdateDynamoDB
{
	GENERATED_BODY()
public:
	FUpdateDynamoDB()
		: ReturnValuesOnConditionCheckFailure(EReturnValuesOnConditionCheckFailure::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::Update() const
	{
		Aws::DynamoDB::Model::Update Data = {};
		for (auto& Element : Key)
			Data.AddKey(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (UpdateExpression.IsEmpty() == false)
			Data.SetUpdateExpression(TCHAR_TO_UTF8(*UpdateExpression));
		if (TableName.IsEmpty() == false)
			Data.SetTableName(TCHAR_TO_UTF8(*TableName));
		if (ConditionExpression.IsEmpty() == false)
			Data.SetConditionExpression(TCHAR_TO_UTF8(*ConditionExpression));
		for (auto& Element : ExpressionAttributeNames)
			Data.AddExpressionAttributeNames(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));
		for (auto& Element : ExpressionAttributeValues)
			Data.AddExpressionAttributeValues(TCHAR_TO_UTF8(*Element.Key), Element.Value);
		if (ReturnValuesOnConditionCheckFailure != EReturnValuesOnConditionCheckFailure::NOT_SET)
			Data.SetReturnValuesOnConditionCheckFailure(static_cast<Aws::DynamoDB::Model::ReturnValuesOnConditionCheckFailure>(ReturnValuesOnConditionCheckFailure));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString UpdateExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnValuesOnConditionCheckFailure ReturnValuesOnConditionCheckFailure;
};

USTRUCT(BlueprintType)
struct FTransactWriteItem
{
	GENERATED_BODY()
public:
	FTransactWriteItem() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::TransactWriteItem() const
	{
		Aws::DynamoDB::Model::TransactWriteItem Data = {};
		Data.SetConditionCheck(ConditionCheck);
		Data.SetPut(Put);
		Data.SetDelete(Delete);
		Data.SetUpdate(Update);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FConditionCheck ConditionCheck;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FPut Put;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDeleteDynamoDB Delete;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FUpdateDynamoDB Update;
};

USTRUCT(BlueprintType)
struct FTransactWriteItemsRequest
{
	GENERATED_BODY()
public:
	FTransactWriteItemsRequest()
		: ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
		  , ReturnItemCollectionMetrics(EReturnItemCollectionMetrics::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FTransactWriteItem> TransactItems;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnItemCollectionMetrics ReturnItemCollectionMetrics;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ClientRequestToken;
};

USTRUCT(BlueprintType)
struct FUntagResourceRequestDynamoDB
{
	GENERATED_BODY()
public:
	FUntagResourceRequestDynamoDB() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ResourceArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FString> TagKeys;
};

USTRUCT(BlueprintType)
struct FPointInTimeRecoverySpecification
{
	GENERATED_BODY()
public:
	FPointInTimeRecoverySpecification()
		: bPointInTimeRecoveryEnabled(false)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::PointInTimeRecoverySpecification() const
	{
		Aws::DynamoDB::Model::PointInTimeRecoverySpecification Data = {};
		Data.SetPointInTimeRecoveryEnabled(bPointInTimeRecoveryEnabled);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bPointInTimeRecoveryEnabled;
};

USTRUCT(BlueprintType)
struct FUpdateContinuousBackupsRequest
{
	GENERATED_BODY()
public:
	FUpdateContinuousBackupsRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FPointInTimeRecoverySpecification PointInTimeRecoverySpecification;
};

USTRUCT(BlueprintType)
struct FUpdateContributorInsightsRequest
{
	GENERATED_BODY()
public:
	FUpdateContributorInsightsRequest()
		: ContributorInsightsAction(EContributorInsightsAction::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EContributorInsightsAction ContributorInsightsAction;
};

USTRUCT(BlueprintType)
struct FCreateReplicaAction
{
	GENERATED_BODY()
public:
	FCreateReplicaAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::CreateReplicaAction() const
	{
		Aws::DynamoDB::Model::CreateReplicaAction Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
};

USTRUCT(BlueprintType)
struct FDeleteReplicaAction
{
	GENERATED_BODY()
public:
	FDeleteReplicaAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::DeleteReplicaAction() const
	{
		Aws::DynamoDB::Model::DeleteReplicaAction Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
};

USTRUCT(BlueprintType)
struct FReplicaUpdate
{
	GENERATED_BODY()
public:
	FReplicaUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ReplicaUpdate() const
	{
		Aws::DynamoDB::Model::ReplicaUpdate Data = {};
		Data.SetCreate(Create);
		Data.SetDelete(Delete);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FCreateReplicaAction Create;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDeleteReplicaAction Delete;
};

USTRUCT(BlueprintType)
struct FProvisionedThroughputOverride
{
	GENERATED_BODY()
public:
	FProvisionedThroughputOverride() = default;
#if !DISABLE_DYNAMODB
	FProvisionedThroughputOverride(const Aws::DynamoDB::Model::ProvisionedThroughputOverride& data)
		: ReadCapacityUnits(LexToString(data.GetReadCapacityUnits()))
	{
	}

public:
	operator Aws::DynamoDB::Model::ProvisionedThroughputOverride() const
	{
		Aws::DynamoDB::Model::ProvisionedThroughputOverride Data = {};
		Data.SetReadCapacityUnits(FCString::Atod(*ReadCapacityUnits));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReadCapacityUnits;
};

USTRUCT(BlueprintType)
struct FReplicaGlobalSecondaryIndex
{
	GENERATED_BODY()
public:
	FReplicaGlobalSecondaryIndex() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndex() const
	{
		Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndex Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		Data.SetProvisionedThroughputOverride(ProvisionedThroughputOverride);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughputOverride ProvisionedThroughputOverride;
};

USTRUCT(BlueprintType)
struct FCreateReplicationGroupMemberAction
{
	GENERATED_BODY()
public:
	FCreateReplicationGroupMemberAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::CreateReplicationGroupMemberAction() const
	{
		Aws::DynamoDB::Model::CreateReplicationGroupMemberAction Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		if (KMSMasterKeyId.IsEmpty() == false)
			Data.SetKMSMasterKeyId(TCHAR_TO_UTF8(*KMSMasterKeyId));
		Data.SetProvisionedThroughputOverride(ProvisionedThroughputOverride);
		for (auto& Element : GlobalSecondaryIndexes)
			Data.AddGlobalSecondaryIndexes(Element);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString KMSMasterKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughputOverride ProvisionedThroughputOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaGlobalSecondaryIndex> GlobalSecondaryIndexes;
};

USTRUCT(BlueprintType)
struct FUpdateReplicationGroupMemberAction
{
	GENERATED_BODY()
public:
	FUpdateReplicationGroupMemberAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::UpdateReplicationGroupMemberAction() const
	{
		Aws::DynamoDB::Model::UpdateReplicationGroupMemberAction Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		if (KMSMasterKeyId.IsEmpty() == false)
			Data.SetKMSMasterKeyId(TCHAR_TO_UTF8(*KMSMasterKeyId));
		Data.SetProvisionedThroughputOverride(ProvisionedThroughputOverride);
		for (auto& Element : GlobalSecondaryIndexes)
			Data.AddGlobalSecondaryIndexes(Element);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString KMSMasterKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughputOverride ProvisionedThroughputOverride;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaGlobalSecondaryIndex> GlobalSecondaryIndexes;
};

USTRUCT(BlueprintType)
struct FDeleteReplicationGroupMemberAction
{
	GENERATED_BODY()
public:
	FDeleteReplicationGroupMemberAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::DeleteReplicationGroupMemberAction() const
	{
		Aws::DynamoDB::Model::DeleteReplicationGroupMemberAction Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
};

USTRUCT(BlueprintType)
struct FReplicationGroupUpdate
{
	GENERATED_BODY()
public:
	FReplicationGroupUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ReplicationGroupUpdate() const
	{
		Aws::DynamoDB::Model::ReplicationGroupUpdate Data = {};
		Data.SetCreate(Create);
		Data.SetUpdate(Update);
		Data.SetDelete(Delete);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FCreateReplicationGroupMemberAction Create;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FUpdateReplicationGroupMemberAction Update;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDeleteReplicationGroupMemberAction Delete;
};

USTRUCT(BlueprintType)
struct FUpdateGlobalTableRequest
{
	GENERATED_BODY()
public:
	FUpdateGlobalTableRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaUpdate> ReplicaUpdates;
};

USTRUCT(BlueprintType)
struct FAutoScalingTargetTrackingScalingPolicyConfigurationUpdate
{
	GENERATED_BODY()
public:
	FAutoScalingTargetTrackingScalingPolicyConfigurationUpdate()
		: bDisableScaleIn(false)
		  , ScaleInCooldown(0)
		  , ScaleOutCooldown(0)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::AutoScalingTargetTrackingScalingPolicyConfigurationUpdate() const
	{
		Aws::DynamoDB::Model::AutoScalingTargetTrackingScalingPolicyConfigurationUpdate Data = {};
		Data.SetDisableScaleIn(bDisableScaleIn);
		if (ScaleInCooldown > 0)
			Data.SetScaleInCooldown(ScaleInCooldown);
		if (ScaleOutCooldown > 0)
			Data.SetScaleOutCooldown(ScaleOutCooldown);
		if (TargetValue.IsEmpty() == false)
			Data.SetTargetValue(FCString::Atod(*TargetValue));
		return Data;
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
struct FAutoScalingPolicyUpdate
{
	GENERATED_BODY()
public:
	FAutoScalingPolicyUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::AutoScalingPolicyUpdate() const
	{
		Aws::DynamoDB::Model::AutoScalingPolicyUpdate Data = {};
		if (PolicyName.IsEmpty() == false)
			Data.SetPolicyName(TCHAR_TO_UTF8(*PolicyName));
		Data.SetTargetTrackingScalingPolicyConfiguration(TargetTrackingScalingPolicyConfiguration);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString PolicyName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingTargetTrackingScalingPolicyConfigurationUpdate TargetTrackingScalingPolicyConfiguration;
};

USTRUCT(BlueprintType)
struct FAutoScalingSettingsUpdate
{
	GENERATED_BODY()
public:
	FAutoScalingSettingsUpdate()
		: bAutoScalingDisabled(false)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::AutoScalingSettingsUpdate() const
	{
		Aws::DynamoDB::Model::AutoScalingSettingsUpdate Data = {};
		if (MinimumUnits.IsEmpty() == false)
			Data.SetMinimumUnits(FCString::Atod(*MinimumUnits));
		if (MinimumUnits.IsEmpty() == false)
			Data.SetMaximumUnits(FCString::Atod(*MaximumUnits));
		Data.SetAutoScalingDisabled(bAutoScalingDisabled);
		if (AutoScalingRoleArn.IsEmpty() == false)
			Data.SetAutoScalingRoleArn(TCHAR_TO_UTF8(*AutoScalingRoleArn));
		Data.SetScalingPolicyUpdate(ScalingPolicyUpdate);
		return Data;
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
	FString AutoScalingRoleArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingPolicyUpdate ScalingPolicyUpdate;
};

USTRUCT(BlueprintType)
struct FGlobalTableGlobalSecondaryIndexSettingsUpdate
{
	GENERATED_BODY()
public:
	FGlobalTableGlobalSecondaryIndexSettingsUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::GlobalTableGlobalSecondaryIndexSettingsUpdate() const
	{
		Aws::DynamoDB::Model::GlobalTableGlobalSecondaryIndexSettingsUpdate Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		if (ProvisionedWriteCapacityUnits.IsEmpty() == false)
			Data.SetProvisionedWriteCapacityUnits(FCString::Atod(*ProvisionedWriteCapacityUnits));
		Data.SetProvisionedWriteCapacityAutoScalingSettingsUpdate(ProvisionedWriteCapacityAutoScalingSettingsUpdate);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProvisionedWriteCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate ProvisionedWriteCapacityAutoScalingSettingsUpdate;
};

USTRUCT(BlueprintType)
struct FReplicaGlobalSecondaryIndexSettingsUpdate
{
	GENERATED_BODY()
public:
	FReplicaGlobalSecondaryIndexSettingsUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndexSettingsUpdate() const
	{
		Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndexSettingsUpdate Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		if (ProvisionedReadCapacityUnits.IsEmpty() == false)
			Data.SetProvisionedReadCapacityUnits(FCString::Atod(*ProvisionedReadCapacityUnits));
		Data.SetProvisionedReadCapacityAutoScalingSettingsUpdate(ProvisionedReadCapacityAutoScalingSettingsUpdate);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ProvisionedReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate ProvisionedReadCapacityAutoScalingSettingsUpdate;
};

USTRUCT(BlueprintType)
struct FReplicaSettingsUpdate
{
	GENERATED_BODY()
public:
	FReplicaSettingsUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ReplicaSettingsUpdate() const
	{
		Aws::DynamoDB::Model::ReplicaSettingsUpdate Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		if (ReplicaProvisionedReadCapacityUnits.IsEmpty() == false)
			Data.SetReplicaProvisionedReadCapacityUnits(FCString::Atod(*ReplicaProvisionedReadCapacityUnits));
		Data.SetReplicaProvisionedReadCapacityAutoScalingSettingsUpdate(ReplicaProvisionedReadCapacityAutoScalingSettingsUpdate);
		for (auto& Element : ReplicaGlobalSecondaryIndexSettingsUpdate)
			Data.AddReplicaGlobalSecondaryIndexSettingsUpdate(Element);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ReplicaProvisionedReadCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate ReplicaProvisionedReadCapacityAutoScalingSettingsUpdate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaGlobalSecondaryIndexSettingsUpdate> ReplicaGlobalSecondaryIndexSettingsUpdate;
};

USTRUCT(BlueprintType)
struct FUpdateGlobalTableSettingsRequest
{
	GENERATED_BODY()
public:
	FUpdateGlobalTableSettingsRequest()
		: GlobalTableBillingMode(EBillingMode::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBillingMode GlobalTableBillingMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString GlobalTableProvisionedWriteCapacityUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate GlobalTableProvisionedWriteCapacityAutoScalingSettingsUpdate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalTableGlobalSecondaryIndexSettingsUpdate> GlobalTableGlobalSecondaryIndexSettingsUpdate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaSettingsUpdate> ReplicaSettingsUpdate;
};

USTRUCT(BlueprintType)
struct FAttributeValueUpdate
{
	GENERATED_BODY()
public:
	FAttributeValueUpdate()
		: Action(EAttributeAction::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::AttributeValueUpdate() const
	{
		Aws::DynamoDB::Model::AttributeValueUpdate Data = {};
		Data.SetValue(Value);
		if (Action != EAttributeAction::NOT_SET)
			Data.SetAction(static_cast<Aws::DynamoDB::Model::AttributeAction>(Action));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAttributeValue Value;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EAttributeAction Action;
};

USTRUCT(BlueprintType)
struct FUpdateItemRequest
{
	GENERATED_BODY()
public:
	FUpdateItemRequest()
		: ConditionalOperator(EConditionalOperator::NOT_SET)
		  , ReturnValues(EReturnValue::NOT_SET)
		  , ReturnConsumedCapacity(EReturnConsumedCapacity::NOT_SET)
		  , ReturnItemCollectionMetrics(EReturnItemCollectionMetrics::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValueUpdate> AttributeUpdates;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FExpectedAttributeValue> Expected;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EConditionalOperator ConditionalOperator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnValue ReturnValues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnConsumedCapacity ReturnConsumedCapacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EReturnItemCollectionMetrics ReturnItemCollectionMetrics;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString UpdateExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString ConditionExpression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FString> ExpressionAttributeNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TMap<FString, FAttributeValue> ExpressionAttributeValues;
};

USTRUCT(BlueprintType)
struct FUpdateGlobalSecondaryIndexAction
{
	GENERATED_BODY()
public:
	FUpdateGlobalSecondaryIndexAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::UpdateGlobalSecondaryIndexAction() const
	{
		Aws::DynamoDB::Model::UpdateGlobalSecondaryIndexAction Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		Data.SetProvisionedThroughput(ProvisionedThroughput);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughput ProvisionedThroughput;
};

USTRUCT(BlueprintType)
struct FCreateGlobalSecondaryIndexAction
{
	GENERATED_BODY()
public:
	FCreateGlobalSecondaryIndexAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::CreateGlobalSecondaryIndexAction() const
	{
		Aws::DynamoDB::Model::CreateGlobalSecondaryIndexAction Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		for (auto& Element : KeySchema)
			Data.AddKeySchema(Element);
		Data.SetProjection(Projection);
		Data.SetProvisionedThroughput(ProvisionedThroughput);
		return Data;
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
struct FDeleteGlobalSecondaryIndexAction
{
	GENERATED_BODY()
public:
	FDeleteGlobalSecondaryIndexAction() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::DeleteGlobalSecondaryIndexAction() const
	{
		Aws::DynamoDB::Model::DeleteGlobalSecondaryIndexAction Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
};

USTRUCT(BlueprintType)
struct FGlobalSecondaryIndexUpdate
{
	GENERATED_BODY()
public:
	FGlobalSecondaryIndexUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::GlobalSecondaryIndexUpdate() const
	{
		Aws::DynamoDB::Model::GlobalSecondaryIndexUpdate Data = {};
		Data.SetUpdate(Update);
		Data.SetCreate(Create);
		Data.SetDelete(Delete);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FUpdateGlobalSecondaryIndexAction Update;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FCreateGlobalSecondaryIndexAction Create;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FDeleteGlobalSecondaryIndexAction Delete;
};

USTRUCT(BlueprintType)
struct FUpdateTableRequest
{
	GENERATED_BODY()
public:
	FUpdateTableRequest()
		: BillingMode(EBillingMode::NOT_SET)
	{
	}

public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::UpdateTableRequest() const
	{
		Aws::DynamoDB::Model::UpdateTableRequest Data = {};
		for (auto& Element : AttributeDefinitions)
			Data.AddAttributeDefinitions(Element);
		if (TableName.IsEmpty() == false)
			Data.SetTableName(TCHAR_TO_UTF8(*TableName));
		if (BillingMode != EBillingMode::NOT_SET)
			Data.SetBillingMode(static_cast<Aws::DynamoDB::Model::BillingMode>(BillingMode));
		Data.SetProvisionedThroughput(ProvisionedThroughput);
		for (auto& Element : GlobalSecondaryIndexUpdates)
			Data.AddGlobalSecondaryIndexUpdates(Element);
		Data.SetStreamSpecification(StreamSpecification);
		Data.SetSSESpecification(SSESpecification);
		for (auto& Element : ReplicaUpdates)
			Data.AddReplicaUpdates(Element);

		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FAttributeDefinition> AttributeDefinitions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	EBillingMode BillingMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FProvisionedThroughput ProvisionedThroughput;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalSecondaryIndexUpdate> GlobalSecondaryIndexUpdates;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FStreamSpecification StreamSpecification;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FSSESpecification SSESpecification;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicationGroupUpdate> ReplicaUpdates;
};

USTRUCT(BlueprintType)
struct FGlobalSecondaryIndexAutoScalingUpdate
{
	GENERATED_BODY()
public:
	FGlobalSecondaryIndexAutoScalingUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::GlobalSecondaryIndexAutoScalingUpdate() const
	{
		Aws::DynamoDB::Model::GlobalSecondaryIndexAutoScalingUpdate Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		Data.SetProvisionedWriteCapacityAutoScalingUpdate(ProvisionedWriteCapacityAutoScalingUpdate);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate ProvisionedWriteCapacityAutoScalingUpdate;
};

USTRUCT(BlueprintType)
struct FReplicaGlobalSecondaryIndexAutoScalingUpdate
{
	GENERATED_BODY()
public:
	FReplicaGlobalSecondaryIndexAutoScalingUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndexAutoScalingUpdate() const
	{
		Aws::DynamoDB::Model::ReplicaGlobalSecondaryIndexAutoScalingUpdate Data = {};
		if (IndexName.IsEmpty() == false)
			Data.SetIndexName(TCHAR_TO_UTF8(*IndexName));
		Data.SetProvisionedReadCapacityAutoScalingUpdate(ProvisionedReadCapacityAutoScalingUpdate);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString IndexName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate ProvisionedReadCapacityAutoScalingUpdate;
};

USTRUCT(BlueprintType)
struct FReplicaAutoScalingUpdate
{
	GENERATED_BODY()
public:
	FReplicaAutoScalingUpdate() = default;
public:
#if !DISABLE_DYNAMODB
	operator Aws::DynamoDB::Model::ReplicaAutoScalingUpdate() const
	{
		Aws::DynamoDB::Model::ReplicaAutoScalingUpdate Data = {};
		if (RegionName.IsEmpty() == false)
			Data.SetRegionName(TCHAR_TO_UTF8(*RegionName));
		for (auto& Element : ReplicaGlobalSecondaryIndexUpdates)
			Data.AddReplicaGlobalSecondaryIndexUpdates(Element);
		Data.SetReplicaProvisionedReadCapacityAutoScalingUpdate(ReplicaProvisionedReadCapacityAutoScalingUpdate);
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString RegionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaGlobalSecondaryIndexAutoScalingUpdate> ReplicaGlobalSecondaryIndexUpdates;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate ReplicaProvisionedReadCapacityAutoScalingUpdate;
};

USTRUCT(BlueprintType)
struct FUpdateTableReplicaAutoScalingRequest
{
	GENERATED_BODY()
public:
	FUpdateTableReplicaAutoScalingRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FGlobalSecondaryIndexAutoScalingUpdate> GlobalSecondaryIndexUpdates;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FAutoScalingSettingsUpdate ProvisionedWriteCapacityAutoScalingUpdate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	TArray<FReplicaAutoScalingUpdate> ReplicaUpdates;
};

USTRUCT(BlueprintType)
struct FTimeToLiveSpecification
{
	GENERATED_BODY()
public:
	FTimeToLiveSpecification()
		: bEnabled(false)
	{
	}
#if !DISABLE_DYNAMODB
	FTimeToLiveSpecification(const Aws::DynamoDB::Model::TimeToLiveSpecification& data)
		: bEnabled(data.GetEnabled())
		  , AttributeName(UTF8_TO_TCHAR(data.GetAttributeName().c_str()))
	{
	}

public:
	operator Aws::DynamoDB::Model::TimeToLiveSpecification() const
	{
		Aws::DynamoDB::Model::TimeToLiveSpecification Data = {};
		Data.SetEnabled(bEnabled);
		if (AttributeName.IsEmpty() == false)
			Data.SetAttributeName(TCHAR_TO_UTF8(*AttributeName));
		return Data;
	}
#endif
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	bool bEnabled;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString AttributeName;
};

USTRUCT(BlueprintType)
struct FUpdateTimeToLiveRequest
{
	GENERATED_BODY()
public:
	FUpdateTimeToLiveRequest() = default;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AwsCore|AwsDynamoDB")
	FTimeToLiveSpecification TimeToLiveSpecification;
};

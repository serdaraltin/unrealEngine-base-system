// Copyright Low Entry. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "LowEntryHttpRequestLibrary.generated.h"


class ULowEntryHttpRequest;


UCLASS()
class LOWENTRYHTTPREQUEST_API ULowEntryHttpRequestLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public: // Request
	/**
	* Creates a new HTTP Request.
	*/
	UFUNCTION(BlueprintCallable, Category = "Low Entry|Http Request|Request", Meta = (DisplayName = "Create (Http Request)"))
	static ULowEntryHttpRequest* Request_Create();


public: // Other
	/**
	* Returns the string URL-encoded (converts slashes to %2F etc).
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Utility", Meta = (DisplayName = "Url Encode", Keywords = "string convert safe"))
	static FString UrlEncode(const FString& String);

	/**
	* Returns the un-URL-encoded string (converts  %2F back to slashes etc).
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Utility", Meta = (DisplayName = "Url Decode", Keywords = "string convert safe"))
	static FString UrlDecode(const FString& String);

	/**
	* Returns the string HTML-encoded (converts > to &gt; etc).
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Utility", Meta = (DisplayName = "Html Encode", Keywords = "string convert safe"))
	static FString HtmlEncode(const FString& String);

	/**
	* Returns the mimetype of the given filepath/extension.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Utility", Meta = (DisplayName = "Get Mimetype", Keywords = "string convert"))
	static FString GetMimeType(const FString& FilePath);

	/**
	* Returns the domain part of the given URL.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Utility", Meta = (DisplayName = "Get Url Domain", Keywords = "string"))
	static FString GetUrlDomain(const FString& Url);

	/**
	* Returns the path part of the given URL.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Utility", Meta = (DisplayName = "Get Url Path", Keywords = "string", AdvancedDisplay = "1"))
	static FString GetUrlPath(const FString& Url, const bool IncludeQueryString = false, const bool IncludeFragment = false);

	/**
	* Returns the given parameter from the given URL, returns an empty string if it doesn't exist.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Utility", Meta = (DisplayName = "Get Url Parameter", Keywords = "string"))
	static FString GetUrlParameter(const FString& Url, const FString& ParameterName);
};

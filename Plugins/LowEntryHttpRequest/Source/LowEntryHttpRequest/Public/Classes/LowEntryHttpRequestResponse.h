// Copyright Low Entry. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

#include "Http.h"

#include "ELowEntryHttpRequestSuccess.h"

#include "LowEntryHttpRequestResponse.generated.h"


UCLASS(BlueprintType)
class LOWENTRYHTTPREQUEST_API ULowEntryHttpRequestResponse : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	static ULowEntryHttpRequestResponse* Create();
	static ULowEntryHttpRequestResponse* Create(const FHttpResponsePtr Response);


private:
	UPROPERTY()
	bool ConnectionSuccessful = false;


public:
	FHttpResponsePtr Response = nullptr;


	void SetResponse(const FHttpResponsePtr NewResponse);


	/**
	* Returns true if the connection was successful.
	* 
	* This blueprint is automatically used in the "Was Request Successful" blueprint.
	*/
	UFUNCTION(BlueprintCallable, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Was Connection Successful", ExpandEnumAsExecs = "Branch", Keywords = "is was"))
	void WasConnectionSuccessful(ELowEntryHttpRequestSuccess& Branch);

	/**
	* Returns true if the connection was successful.
	* 
	* This blueprint is automatically used in the "Was Request Successful" blueprint.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Was Connection Successful", Keywords = "is was"))
	bool GetWasConnectionSuccessful();

	void SetWasConnectionSuccessful(const bool WasSuccessful);


	/**
	* Returns true if the HTTP request was successful.
	* 
	* This blueprint just runs the "Was Connection Successful" and the "Is Response Code Ok" blueprints, it will return true if both of these blueprints returned true.
	*/
	UFUNCTION(BlueprintCallable, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Was Request Successful", ExpandEnumAsExecs = "Branch", Keywords = "is was"))
	void WasSuccessful(ELowEntryHttpRequestSuccess& Branch);

	/**
	* Returns true if the HTTP request was successful.
	* 
	* This blueprint just runs the "Was Connection Successful" and the "Is Response Code Ok" blueprints, it will return true if both of these blueprints returned true.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Was Request Successful", Keywords = "is was"))
	bool GetWasSuccessful();


	/**
	* Returns the HTTP response code.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Response Code", Keywords = "is was"))
	int32 GetResponseCode();

	/**
	* Returns true if the HTTP response code is 'OK' according to the engine. The current implementation checks if the HTTP response code is between 200 and 206. Returns false otherwise.
	* 
	* This blueprint is automatically used in the "Was Request Successful" blueprint.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Is Response Code Ok", Keywords = "is was"))
	bool IsResponseCodeOk();


	/**
	* Returns the URL to which the HTTP request was made.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Url"))
	FString GetUrl();


	/**
	* Returns the content as bytes that the HTTP request returned.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Content As Bytes"))
	TArray<uint8> GetContent();

	/**
	* Returns the content as a string that the HTTP request returned.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Content As String"))
	FString GetContentAsString();

	/**
	* Returns the length of the content that the HTTP request returned.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Content Length"))
	int32 GetContentLength();

	/**
	* Returns the type of the content that the HTTP request returned.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Content Type"))
	FString GetContentType();


	/**
	* Returns a header that the HTTP request returned.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get Header"))
	FString GetHeader(const FString& Key);

	/**
	* Returns all headers that the HTTP request returned.
	*/
	UFUNCTION(BlueprintPure, Category = "Low Entry|Http Request|Response", Meta = (DisplayName = "Get All Headers"))
	TArray<FString> GetAllHeaders();
};

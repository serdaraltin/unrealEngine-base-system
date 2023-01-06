// Copyright 2017 David Romanski(Socke). All Rights Reserved.

#pragma once

#include "JSONParser.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "JSONParserAsyncObjectToString.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectToStringCompleteDelegate, const FString&, json);

UCLASS()
class JSONPARSER_API UJSONParserAsyncObjectToString : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		
public:


	UPROPERTY(BlueprintAssignable)
		FObjectToStringCompleteDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FObjectToStringCompleteDelegate OnFail;


	UFUNCTION(BlueprintCallable, Category = "JSON", meta = (BlueprintInternalUseOnly = "true"))
		static UJSONParserAsyncObjectToString* JsonObjectToStringAsync(struct FSJsonObject mainObject, bool removeLineBreaks = true, bool addPadding = false);

	UFUNCTION(BlueprintCallable, Category = "JSON", meta = (BlueprintInternalUseOnly = "true"))
		static UJSONParserAsyncObjectToString* JsonObjectToFileAsync(struct FSJsonObject mainObject, EJSONDirectoryType directoryType,FString filePath, bool removeLineBreaks = true, bool addPadding = false);



	virtual void Activate() override;

	void jobComplete(const FString json, bool successful);

private:
	FSJsonThreadJobData jobData;
	FJSONParserThread* jsonThread = nullptr;
};
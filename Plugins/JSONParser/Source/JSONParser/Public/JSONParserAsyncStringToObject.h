// Copyright 2017 David Romanski(Socke). All Rights Reserved.

#pragma once

#include "JSONParser.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "JSONParserAsyncStringToObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStringToObjectCompleteDelegate, const struct FSJsonObject&, jsonObject);

UCLASS()
class JSONPARSER_API UJSONParserAsyncStringToObject : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		
public:


	UPROPERTY(BlueprintAssignable)
		FStringToObjectCompleteDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FStringToObjectCompleteDelegate OnFail;


	UFUNCTION(BlueprintCallable, Category = "JSON", meta = (BlueprintInternalUseOnly = "true"))
		static UJSONParserAsyncStringToObject* JsonStringToObjectAsync(FString JSON, bool removeAndSavePadding);

	UFUNCTION(BlueprintCallable, Category = "JSON", meta = (BlueprintInternalUseOnly = "true"))
		static UJSONParserAsyncStringToObject* JsonFileToObjectAsync(EJSONDirectoryType directoryType, FString filePath, bool removeAndSavePadding);


	virtual void Activate() override;

	void jobComplete(const struct FSJsonObject jsonObject, bool successful);

private:
	FSJsonThreadJobData jobData;
	FJSONParserThread* jsonThread = nullptr;
};
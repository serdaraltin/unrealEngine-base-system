// Copyright Low Entry. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

#include "ELowEntryHttpRequestSuccess.generated.h"


UENUM(BlueprintType)
enum class ELowEntryHttpRequestSuccess : uint8
{
	Success = 0,
	Failed = 1
};

// Copyright Low Entry. All Rights Reserved.

#include "LowEntryHttpRequestResponse.h"


ULowEntryHttpRequestResponse::ULowEntryHttpRequestResponse(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

ULowEntryHttpRequestResponse* ULowEntryHttpRequestResponse::Create()
{
	return NewObject<ULowEntryHttpRequestResponse>();
}

ULowEntryHttpRequestResponse* ULowEntryHttpRequestResponse::Create(const FHttpResponsePtr Response)
{
	ULowEntryHttpRequestResponse* Instance = NewObject<ULowEntryHttpRequestResponse>();
	Instance->Response = Response;
	return Instance;
}


void ULowEntryHttpRequestResponse::SetResponse(const FHttpResponsePtr NewResponse)
{
	Response = NewResponse;
}


void ULowEntryHttpRequestResponse::WasConnectionSuccessful(ELowEntryHttpRequestSuccess& Branch)
{
	if (ConnectionSuccessful)
	{
		Branch = ELowEntryHttpRequestSuccess::Success;
	}
	else
	{
		Branch = ELowEntryHttpRequestSuccess::Failed;
	}
}

bool ULowEntryHttpRequestResponse::GetWasConnectionSuccessful()
{
	return ConnectionSuccessful;
}

void ULowEntryHttpRequestResponse::SetWasConnectionSuccessful(const bool WasSuccessful)
{
	ConnectionSuccessful = WasSuccessful;
}


void ULowEntryHttpRequestResponse::WasSuccessful(ELowEntryHttpRequestSuccess& Branch)
{
	if (GetWasSuccessful())
	{
		Branch = ELowEntryHttpRequestSuccess::Success;
	}
	else
	{
		Branch = ELowEntryHttpRequestSuccess::Failed;
	}
}

bool ULowEntryHttpRequestResponse::GetWasSuccessful()
{
	return (GetWasConnectionSuccessful() && IsResponseCodeOk());
}


int32 ULowEntryHttpRequestResponse::GetResponseCode()
{
	if (!Response.IsValid())
	{
		return -1;
	}
	return Response->GetResponseCode();
}

bool ULowEntryHttpRequestResponse::IsResponseCodeOk()
{
	if (!Response.IsValid())
	{
		return false;
	}
	return EHttpResponseCodes::IsOk(Response->GetResponseCode());
}


FString ULowEntryHttpRequestResponse::GetUrl()
{
	if (!Response.IsValid())
	{
		return TEXT("");
	}
	return Response->GetURL();
}


TArray<uint8> ULowEntryHttpRequestResponse::GetContent()
{
	if (!Response.IsValid())
	{
		return TArray<uint8>();
	}
	return Response->GetContent();
}

FString ULowEntryHttpRequestResponse::GetContentAsString()
{
	if (!Response.IsValid())
	{
		return TEXT("");
	}
	return Response->GetContentAsString();
}

int32 ULowEntryHttpRequestResponse::GetContentLength()
{
	if (!Response.IsValid())
	{
		return 0;
	}
	int64 Size = Response->GetContent().Num();
	if (Size > 0x7fffffff)
	{
		return 0x7fffffff;
	}
	return Size;
}

FString ULowEntryHttpRequestResponse::GetContentType()
{
	if (!Response.IsValid())
	{
		return TEXT("");
	}
	return Response->GetContentType();
}


FString ULowEntryHttpRequestResponse::GetHeader(const FString& Key)
{
	if (!Response.IsValid())
	{
		return TEXT("");
	}
	return Response->GetHeader(Key);
}

TArray<FString> ULowEntryHttpRequestResponse::GetAllHeaders()
{
	if (!Response.IsValid())
	{
		return TArray<FString>();
	}
	return Response->GetAllHeaders();
}

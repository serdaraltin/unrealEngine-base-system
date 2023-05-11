// Copyright Low Entry. All Rights Reserved.

#include "LowEntryHttpRequestLibrary.h"
#include "LowEntryHttpRequest.h"


ULowEntryHttpRequestLibrary::ULowEntryHttpRequestLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}


ULowEntryHttpRequest* ULowEntryHttpRequestLibrary::Request_Create()
{
	return ULowEntryHttpRequest::Create();
}


// other >>
FString ULowEntryHttpRequestLibrary::UrlEncode(const FString& String)
{
	return FGenericPlatformHttp::UrlEncode(String);
}

FString ULowEntryHttpRequestLibrary::UrlDecode(const FString& String)
{
	return FGenericPlatformHttp::UrlDecode(String);
}

FString ULowEntryHttpRequestLibrary::HtmlEncode(const FString& String)
{
	return FGenericPlatformHttp::HtmlEncode(String);
}

FString ULowEntryHttpRequestLibrary::GetMimeType(const FString& FilePath)
{
	return FGenericPlatformHttp::GetMimeType(FilePath);
}

FString ULowEntryHttpRequestLibrary::GetUrlDomain(const FString& Url)
{
	return FGenericPlatformHttp::GetUrlDomain(Url);
}

FString ULowEntryHttpRequestLibrary::GetUrlPath(const FString& Url, const bool IncludeQueryString, const bool IncludeFragment)
{
	return FGenericPlatformHttp::GetUrlPath(Url, IncludeQueryString, IncludeFragment);
}

FString ULowEntryHttpRequestLibrary::GetUrlParameter(const FString& Url, const FString& ParameterName)
{
	TOptional<FString> Result = FGenericPlatformHttp::GetUrlParameter(Url, ParameterName);
	if (!Result.IsSet())
	{
		return TEXT("");
	}
	return Result.GetValue();
}
// other <<

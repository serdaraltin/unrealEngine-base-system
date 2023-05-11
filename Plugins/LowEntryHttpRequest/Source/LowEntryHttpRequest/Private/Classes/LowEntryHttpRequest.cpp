// Copyright Low Entry. All Rights Reserved.

#include "LowEntryHttpRequest.h"
#include "LowEntryHttpRequestResponse.h"
#include "FLowEntryHttpRequestExecutionAction.h"

#include "Engine/Engine.h"
#include "Misc/Base64.h"


ULowEntryHttpRequest::ULowEntryHttpRequest(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetHeader(TEXT("content-type"), TEXT("text/plain"));
	HttpRequest->SetContent(TArray<uint8>());
}

ULowEntryHttpRequest* ULowEntryHttpRequest::Create()
{
	return NewObject<ULowEntryHttpRequest>();
}


void ULowEntryHttpRequest::SetUrl(const ELowEntryHttpRequestType Type, const FString& Url)
{
	HttpRequest->SetVerb(ELowEntryHttpRequestTypes[static_cast<int32>(Type)]);
	GivenUrl = Url;
}

void ULowEntryHttpRequest::SetUrlCustomType(const FString& Type, const FString& Url)
{
	HttpRequest->SetVerb(Type.TrimStartAndEnd().ToUpper());
	GivenUrl = Url.TrimStartAndEnd();
}

void ULowEntryHttpRequest::SetUrlParameter(const FString& Key, const FString& Value)
{
	UrlParameters.Add(Key, Value);
}

void ULowEntryHttpRequest::SetHeader(const FString& Key, const FString& Value)
{
	FString _Key = Key.TrimStartAndEnd().ToLower();
	HttpRequest->SetHeader(_Key, Value);
	Headers.Add(_Key, Value);
}


void ULowEntryHttpRequest::SetContent(const TArray<uint8>& Data)
{
	HttpRequest->SetContent(Data);
	ContentStringParameters.Empty();
	ContentMultipartParameters.Empty();
	JsonData = false;
}

void ULowEntryHttpRequest::SetContentAsString(const FString& Data)
{
	HttpRequest->SetContentAsString(Data);
	ContentStringParameters.Empty();
	ContentMultipartParameters.Empty();
	JsonData = false;
}

void ULowEntryHttpRequest::SetContentAsJsonString(const FString& Data)
{
	HttpRequest->SetContentAsString(Data);
	ContentStringParameters.Empty();
	ContentMultipartParameters.Empty();
	JsonData = true;
}

void ULowEntryHttpRequest::SetContentAsStringParameters(const FString& Key, const FString& Value)
{
	ContentStringParameters.Add(Key.TrimStartAndEnd().ToLower(), Value);
	ContentMultipartParameters.Empty();
	JsonData = false;
}

void ULowEntryHttpRequest::SetContentAsMultipartParameters(const FString& Name, const FString& Content, const FString& OptionalContentType, const FString& OptionalFileName)
{
	FString Value = TEXT("Content-Disposition:form-data;name=\"") + Name.TrimStartAndEnd() + TEXT("\"") + (OptionalFileName.TrimStartAndEnd().IsEmpty() ? TEXT("") : TEXT(";filename=\"") + OptionalFileName.TrimStartAndEnd() + TEXT("\"")) + TEXT("\r\n") + TEXT("Content-type:") + (OptionalContentType.TrimStartAndEnd().IsEmpty() ? TEXT("text/plain") : OptionalContentType.TrimStartAndEnd()) + TEXT("\r\n") + TEXT("\r\n") + Content;
	TArray<uint8> ValueBytes;
	FTCHARToUTF8 Src = FTCHARToUTF8(Value.GetCharArray().GetData());
	ValueBytes.Append((uint8*)Src.Get(), Src.Length());
	ContentMultipartParameters.Add(Name.TrimStartAndEnd().ToLower(), FBase64::Encode(ValueBytes));
	GeneratedContentMultipartBoundary.Empty();

	ContentStringParameters.Empty();
	JsonData = false;
}

void ULowEntryHttpRequest::SetContentAsMultipartParametersBytes(const FString& Name, const TArray<uint8>& Content, const FString& OptionalContentType, const FString& OptionalFileName)
{
	FString Value = TEXT("Content-Disposition:form-data;name=\"") + Name.TrimStartAndEnd() + TEXT("\"") + (OptionalFileName.TrimStartAndEnd().IsEmpty() ? TEXT("") : TEXT(";filename=\"") + OptionalFileName.TrimStartAndEnd() + TEXT("\"")) + TEXT("\r\n") + TEXT("Content-type:") + (OptionalContentType.TrimStartAndEnd().IsEmpty() ? TEXT("application/octet-stream") : OptionalContentType.TrimStartAndEnd()) + TEXT("\r\n") + TEXT("\r\n");
	TArray<uint8> ValueBytes;
	FTCHARToUTF8 Src = FTCHARToUTF8(Value.GetCharArray().GetData());
	ValueBytes.Append((uint8*)Src.Get(), Src.Length());
	ValueBytes.Append(Content);
	ContentMultipartParameters.Add(Name.TrimStartAndEnd().ToLower(), FBase64::Encode(ValueBytes));
	GeneratedContentMultipartBoundary.Empty();

	ContentStringParameters.Empty();
	JsonData = false;
}


void ULowEntryHttpRequest::ExecuteRequest(UObject* WorldContextObject, FLatentActionInfo LatentInfo, ULowEntryHttpRequestResponse*& Response)
{
	if (IsExecutingRequest())
	{
		Response = ExecutionResponse;
		return;
	}

	Response = ULowEntryHttpRequestResponse::Create();

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		return;
	}
	if (World->GetLatentActionManager().FindExistingAction<FLowEntryHttpRequestExecutionAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) != nullptr)
	{
		return;
	}

	ProcessData();

	KeepAliveCount++;
	if (KeepAliveCount == 1)
	{
		AddToRoot();
	}

	BytesToSend = HttpRequest->GetContentLength();
	BytesSent = 0;
	BytesToReceive = 0;
	BytesReceived = 0;

	ExecutionResponse = Response;
	Execution = new FLowEntryHttpRequestExecutionAction(LatentInfo, HttpRequest->GetURL(), this, Response);
	World->GetLatentActionManager().AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, Execution);

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULowEntryHttpRequest::OnExecuteRequestCompleted);
	HttpRequest->OnRequestProgress().BindUObject(this, &ULowEntryHttpRequest::OnExecuteRequestProgress);
	HttpRequest->ProcessRequest();
}

void ULowEntryHttpRequest::OnExecuteRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	BytesSent = BytesToSend;
	BytesReceived = BytesToReceive;

	if (ExecutionResponse != nullptr)
	{
		ExecutionResponse->SetResponse(Response);
		ExecutionResponse->SetWasConnectionSuccessful(bWasSuccessful);
		//ExecutionResponse = nullptr;

		if (Execution != nullptr)
		{
			Execution->Finish();
		}
	}
	else
	{
		ExecutionActionIsDone();
	}
}

void ULowEntryHttpRequest::ExecutionActionIsDone()
{
	if (Execution != nullptr)
	{
		Execution->HttpResponse = ExecutionResponse;
		ExecutionResponse = nullptr;

		Execution = nullptr;

		KeepAliveCount--;
		if (KeepAliveCount == 0)
		{
			RemoveFromRoot();
		}
		if (KeepAliveCount < 0)
		{
			KeepAliveCount = 0;
		}
	}
}


void ULowEntryHttpRequest::ExecuteRequestEvent(const FDelegateULowEntryHttpRequestOnComplete OnComplete)
{
	ExecutionEvent = OnComplete;
	if (ExecutionEvent.IsBound())
	{
		ExecutionEventObject = ExecutionEvent.GetUObject();
	}

	if (IsExecutingRequest())
	{
		return;
	}

	ExecutionResponse = ULowEntryHttpRequestResponse::Create();

	ProcessData();

	KeepAliveCount++;
	if (KeepAliveCount == 1)
	{
		AddToRoot();
	}

	BytesToSend = HttpRequest->GetContentLength();
	BytesSent = 0;
	BytesToReceive = 0;
	BytesReceived = 0;

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULowEntryHttpRequest::OnExecuteRequestCompletedEvent);
	HttpRequest->OnRequestProgress().BindUObject(this, &ULowEntryHttpRequest::OnExecuteRequestProgress);
	HttpRequest->ProcessRequest();
}

void ULowEntryHttpRequest::OnExecuteRequestCompletedEvent(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	BytesSent = BytesToSend;
	BytesReceived = BytesToReceive;

	if (ExecutionResponse != nullptr)
	{
		ExecutionResponse->SetResponse(Response);
		ExecutionResponse->SetWasConnectionSuccessful(bWasSuccessful);
		//ExecutionResponse = nullptr;
	}

	KeepAliveCount--;
	if (KeepAliveCount == 0)
	{
		RemoveFromRoot();
	}
	if (KeepAliveCount < 0)
	{
		KeepAliveCount = 0;
	}

	if (ExecutionResponse != nullptr)
	{
		ExecutionEvent.ExecuteIfBound(ExecutionResponse);
	}
	ExecutionEventObject = nullptr;
}


void ULowEntryHttpRequest::OnExecuteRequestProgress(FHttpRequestPtr Request, int32 NewBytesSent, int32 NewBytesReceived)
{
	BytesSent = NewBytesSent;
	BytesReceived = NewBytesReceived;

	if (Request.IsValid() && Request->GetResponse().IsValid())
	{
		BytesToReceive = Request->GetResponse()->GetContentLength();
	}
}


void ULowEntryHttpRequest::CancelRequest()
{
	HttpRequest->CancelRequest();
}

bool ULowEntryHttpRequest::IsExecutingRequest()
{
	return (Execution != nullptr);
}


void ULowEntryHttpRequest::ProcessData()
{
	HttpRequest->SetURL(GetUrlWithUrlParameters());

	if (ContentMultipartParameters.Num() > 0)
	{
		FString Boundary;
		TArray<uint8> Content;
		GetContentAsMultipartParameters(Boundary, Content);
		SetHeader(TEXT("content-type"), TEXT("multipart/form-data;charset=utf-8;boundary=\"") + Boundary + TEXT("\""));
		HttpRequest->SetContent(Content);
	}
	else if (ContentStringParameters.Num() > 0)
	{
		SetHeader(TEXT("content-type"), TEXT("application/x-www-form-urlencoded"));
		HttpRequest->SetContentAsString(GetContentAsStringParameters());
	}
	else if (JsonData)
	{
		SetHeader(TEXT("content-type"), TEXT("application/json;charset=utf-8"));
	}
}

FString ULowEntryHttpRequest::GetUrlWithUrlParameters()
{
	FString Url = GivenUrl;

	if (UrlParameters.Num() <= 0)
	{
		return Url;
	}

	int32 LastSlash;
	int32 LastQuestionMark;
	Url.FindLastChar('/', LastSlash);
	Url.FindLastChar('?', LastQuestionMark);

	if ((LastQuestionMark <= -1) || (LastQuestionMark < LastSlash)) // ? is missing
	{
		if (UrlParameters.Num() > 0)
		{
			Url += '?';
		}
	}

	if (!Url.EndsWith(TEXT("?")) && !Url.EndsWith(TEXT("&"))) // last character is not a ? or &
	{
		Url += '&';
	}

	for (const auto& Entry : UrlParameters)
	{
		if (Entry.Key.Len() > 0)
		{
			Url += FGenericPlatformHttp::UrlEncode(Entry.Key);
			Url += '=';
			Url += FGenericPlatformHttp::UrlEncode(Entry.Value);
			Url += '&';
		}
	}

	if (Url.EndsWith(TEXT("&")))
	{
		Url.RemoveFromEnd(TEXT("&"));
	}

	return Url;
}

FString ULowEntryHttpRequest::GetContentAsStringParameters()
{
	FString Content;

	if (ContentStringParameters.Num() <= 0)
	{
		return Content;
	}

	for (const auto& Entry : ContentStringParameters)
	{
		if (Entry.Key.Len() > 0)
		{
			Content += FGenericPlatformHttp::UrlEncode(Entry.Key);
			Content += '=';
			Content += FGenericPlatformHttp::UrlEncode(Entry.Value);
			Content += '&';
		}
	}

	if (Content.EndsWith(TEXT("&")))
	{
		Content.RemoveFromEnd(TEXT("&"));
	}

	return Content;
}

void ULowEntryHttpRequest::GetContentAsMultipartParameters(FString& Boundary, TArray<uint8>& Content)
{
	TArray<uint8> BoundaryBytes;

	if (GeneratedContentMultipartBoundary.IsEmpty())
	{
		FString Characters = TEXT("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
		while (true)
		{
			Boundary = TEXT("LowEntry-");
			for (int32 i = 0; i < 20; i++)
			{
				Boundary += Characters[FMath::Rand() % Characters.Len()];
			}
			BoundaryBytes = TArray<uint8>();
			FTCHARToUTF8 Src = FTCHARToUTF8(Boundary.GetCharArray().GetData());
			BoundaryBytes.Append((uint8*)Src.Get(), Src.Length());

			bool Found = false;
			for (const auto& Entry : ContentMultipartParameters)
			{
				TArray<uint8> EntryValueBytes;
				if (!FBase64::Decode(Entry.Value, EntryValueBytes))
				{
					continue;
				}
				for (int64 i = 0; i < (EntryValueBytes.Num() - BoundaryBytes.Num()); i++)
				{
					for (int64 j = 0; j < BoundaryBytes.Num(); j++)
					{
						if (EntryValueBytes[i + j] != BoundaryBytes[j])
						{
							break;
						}
						if (j == (BoundaryBytes.Num() - 1))
						{
							Found = true;
							break;
						}
					}
					if (Found)
					{
						break;
					}
				}
				if (Found)
				{
					break;
				}
			}

			if (!Found)
			{
				break;
			}
		}
		GeneratedContentMultipartBoundary = Boundary;
	}
	else
	{
		Boundary = GeneratedContentMultipartBoundary;
		BoundaryBytes = TArray<uint8>();
		FTCHARToUTF8 Src = FTCHARToUTF8(Boundary.GetCharArray().GetData());
		BoundaryBytes.Append((uint8*)Src.Get(), Src.Length());
	}

	TArray<uint8> DashDashBytes;
	{
		FString String = TEXT("--");
		FTCHARToUTF8 Src = FTCHARToUTF8(String.GetCharArray().GetData());
		DashDashBytes.Append((uint8*)Src.Get(), Src.Length());
	}

	TArray<uint8> NewlineBytes;
	{
		FString String = TEXT("\r\n");
		FTCHARToUTF8 Src = FTCHARToUTF8(String.GetCharArray().GetData());
		NewlineBytes.Append((uint8*)Src.Get(), Src.Length());
	}

	Content = TArray<uint8>();
	for (const auto& Entry : ContentMultipartParameters)
	{
		TArray<uint8> EntryValueBytes;
		if (!FBase64::Decode(Entry.Value, EntryValueBytes))
		{
			continue;
		}
		Content.Append(DashDashBytes);
		Content.Append(BoundaryBytes);
		Content.Append(NewlineBytes);
		Content.Append(EntryValueBytes);
		Content.Append(NewlineBytes);
	}
	Content.Append(DashDashBytes);
	Content.Append(BoundaryBytes);
	Content.Append(DashDashBytes);
	Content.Append(NewlineBytes);
}


void ULowEntryHttpRequest::GetUrl(FString& Type, FString& Url)
{
	ProcessData();
	Url = HttpRequest->GetURL();
	Type = HttpRequest->GetVerb();
}

TMap<FString, FString> ULowEntryHttpRequest::GetHeaders()
{
	ProcessData();
	return Headers;
}

TArray<uint8> ULowEntryHttpRequest::GetContent()
{
	ProcessData();
	return HttpRequest->GetContent();
}


int32 ULowEntryHttpRequest::GetBytesToSend()
{
	return BytesToSend;
}

int32 ULowEntryHttpRequest::GetBytesSent()
{
	if (BytesSent >= BytesToSend)
	{
		return BytesToSend;
	}
	return BytesSent;
}

int32 ULowEntryHttpRequest::GetBytesToReceive()
{
	return BytesToReceive;
}

int32 ULowEntryHttpRequest::GetBytesReceived()
{
	if (BytesReceived >= BytesToReceive)
	{
		return BytesToReceive;
	}
	return BytesReceived;
}

double ULowEntryHttpRequest::GetSentPercentage()
{
	if (BytesToSend <= 0)
	{
		return 1.0;
	}
	if (BytesSent >= BytesToSend)
	{
		return 1.0;
	}
	return (static_cast<double>(BytesSent) / static_cast<double>(BytesToSend));
}

double ULowEntryHttpRequest::GetReceivedPercentage()
{
	if (BytesToReceive <= 0)
	{
		return 1.0;
	}
	if (BytesReceived >= BytesToReceive)
	{
		return 1.0;
	}
	return (static_cast<double>(BytesReceived) / static_cast<double>(BytesToReceive));
}

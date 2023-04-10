// Copyright 2020 ByOwls. All Rights Reserved.

#include "TCPNetwork.h"
#include "VideoStreaming.h"
#include "StreamCommunication.h"
#include "StreamSettings.h"
#include "ImageCompresser.h"
#include "IImageWrapper.h"

TCPNetwork::TCPNetwork()
{
}

TCPNetwork::~TCPNetwork()
{
}

//===============================================================================================

void FStreamTCPConnector::Start()
{
	Thread = FRunnableThread::Create(this, TEXT("Video Stream"), 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

uint32 FStreamTCPConnector::Run()
{
	Socket->SetNonBlocking(false);

	const bool bResult = Socket->Connect(*Addr);
	OnConnectDelegate.ExecuteIfBound(bResult);

	return 0;
}

bool FStreamTCPConnector::Init()
{
	return true;
}


FStreamTCPConnector::FStreamTCPConnector(StreamCommunication* InStreaming, FSocket* InSocket, TSharedRef<FInternetAddr> InAddr)
	: Streaming(InStreaming)
	, Socket(InSocket)
	, Addr(InAddr)
	, WaitTime(5000.f)
{
	check(InSocket != nullptr);

}

FStreamTCPConnector::~FStreamTCPConnector()
{
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

void FStreamTCPConnector::Stop()
{
	Socket->Close();
	Thread->WaitForCompletion();
}


//===============================================================================================


FStreamTCPReceiver::~FStreamTCPReceiver()
{
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

uint32 FStreamTCPReceiver::Run()
{
	while (!bIsStopping) {

		if (!Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
		{
			// Receiver time out
			continue;
		}

		uint32 Size;

		while (Socket->HasPendingData(Size))
		{
			FArrayReaderPtr Reader = MakeShareable(new FArrayReader(true));
			Reader->SetNumUninitialized(FMath::Min(Size, 65507u));

			int32 Read = 0;
			Socket->Recv(Reader->GetData(), Reader->Num(), Read);
			Reader->SetNum(Read);

			OnReceivedDelegate.ExecuteIfBound(Reader);
		}
	}

	return 0;
}

void FStreamTCPReceiver::Stop()
{
	bIsStopping = true;
	Thread->WaitForCompletion();
}

bool FStreamTCPReceiver::Init()
{
	return true;
}



FStreamTCPReceiver::FStreamTCPReceiver(StreamCommunication* InStreaming, FSocket* InSocket, const TCHAR* ThreadDescription)
	: Streaming(InStreaming)
	, Socket(InSocket)
	, WaitTime(FTimespan::FromMilliseconds(100))
	, bIsStopping(false)
{

	Thread = FRunnableThread::Create(this, ThreadDescription, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

//===============================================================================================


uint32 FStreamTCPSender::Run()
{
	while (!Stopping)
	{
		while (!Stopping && !SendQueue.IsEmpty())
		{
			if (Socket->Wait(ESocketWaitConditions::WaitForWrite, WaitTime))
			{
				FPacket Packet;
				int32 Sent = 0;

				SendQueue.Dequeue(Packet);
				Socket->Send(Packet.Data->GetData(), Packet.Data->Num(), Sent);

				if (Sent != Packet.Data->Num())
				{
					Stopping = true;

					return 0;
				}
			}
		}

		WorkEvent->Wait(WaitTime);
	}

	Socket->Close();

	return 0;
}


bool FStreamTCPSender::Send(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& Data)
{
	if (!Stopping && SendQueue.Enqueue(FPacket(Data)))
	{
		WorkEvent->Trigger();

		return true;
	}

	return false;
}

FStreamTCPSender::~FStreamTCPSender()
{
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}

	FPlatformProcess::ReturnSynchEventToPool(WorkEvent);
	WorkEvent = nullptr;
}


FStreamTCPSender::FStreamTCPSender(StreamCommunication* InStreaming, FSocket* InSocket, const TCHAR* ThreadDescription)
	: Streaming(InStreaming)
{
	SendRate = 0;
	Socket = InSocket;
	Stopping = false;
	WaitTime = FTimespan::FromMilliseconds(100);

	check(Socket != NULL);
	check(Socket->GetSocketType() == SOCKTYPE_Streaming);

	int32 NewSize = 0;
	Socket->SetSendBufferSize(512 * 1024, NewSize);

	WorkEvent = FPlatformProcess::GetSynchEventFromPool();

	Thread = FRunnableThread::Create(this, ThreadDescription, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void FStreamTCPSender::Stop()
{
	Stopping = true;

	WorkEvent->Trigger();
	Thread->WaitForCompletion();
}
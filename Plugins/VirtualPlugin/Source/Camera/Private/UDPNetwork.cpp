// Copyright 2020 ByOwls. All Rights Reserved.

#include "UDPNetwork.h"
#include "VideoStreaming.h"
#include "StreamCommunication.h"
#include "StreamSettings.h"
#include "ImageCompresser.h"
#include "IImageWrapper.h"

UDPNetwork::UDPNetwork()
{
}

UDPNetwork::~UDPNetwork()
{
}

//===========================================================================================

uint32 FStreamUDPReceiver::Run()
{
	auto SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	TSharedPtr<FInternetAddr> RecvAddr = SocketSubsystem->CreateInternetAddr();

	while (!bIsStopping) {

		if (Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
		{
			uint32 Size;

			while (Socket->HasPendingData(Size))
			{
				FArrayReaderPtr Reader = MakeShareable(new FArrayReader(true));
				Reader->SetNumUninitialized(FMath::Min(Size, 65507u));

				int32 Read = 0;
				Socket->RecvFrom(Reader->GetData(), Reader->Num(), Read, *RecvAddr);
				Reader->SetNum(Read);

				OnReceivedDelegate.ExecuteIfBound(Reader, FIPv4Endpoint(RecvAddr));
			}
		}
	}

	return 0;
}

bool FStreamUDPReceiver::Init()
{
	return true;
}

void FStreamUDPReceiver::Stop()
{
	bIsStopping = true;
	Thread->WaitForCompletion();
}



FStreamUDPReceiver::FStreamUDPReceiver(FSocket* InSocket, const TCHAR* ThreadDescription)
{
	Socket = InSocket;
	WaitTime=FTimespan::FromMilliseconds(100);
	bIsStopping=false;

	Thread = FRunnableThread::Create(this, ThreadDescription, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

FStreamUDPReceiver::~FStreamUDPReceiver()
{
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}



//==============================================================================================================

uint32 FStreamUDPSender::Run()
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
				Socket->SendTo(Packet.Data->GetData(), Packet.Data->Num(), Sent, *Packet.Recipient.ToInternetAddr());
			}
		}

		WorkEvent->Wait(WaitTime);
	}

	while (!SendQueue.IsEmpty())
	{
		if (Socket->Wait(ESocketWaitConditions::WaitForWrite, WaitTime))
		{
			FPacket Packet;
			int32 Sent = 0;

			SendQueue.Dequeue(Packet);
			Socket->SendTo(Packet.Data->GetData(), Packet.Data->Num(), Sent, *Packet.Recipient.ToInternetAddr());
		}
		else
		{
			break;
		}
	}

	return 0;
}

void FStreamUDPSender::Stop()
{
	Stopping = true;
	WorkEvent->Trigger();
	Thread->WaitForCompletion();
}



FStreamUDPSender::~FStreamUDPSender()
{
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}

	FPlatformProcess::ReturnSynchEventToPool(WorkEvent);
	WorkEvent = nullptr;
}

FStreamUDPSender::FStreamUDPSender(FSocket* InSocket, const TCHAR* ThreadDescription)
{
	SendRate = 0;
	Socket = InSocket;
	Stopping = false;
	WaitTime = FTimespan::FromMilliseconds(100);

	check(Socket != NULL);
	check(Socket->GetSocketType() == SOCKTYPE_Datagram);

	int32 NewSize = 0;
	Socket->SetSendBufferSize(512 * 1024, NewSize);

	WorkEvent = FPlatformProcess::GetSynchEventFromPool();

	Thread = FRunnableThread::Create(this, ThreadDescription, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

bool FStreamUDPSender::Send(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Recipient)
{
	if (!Stopping && SendQueue.Enqueue(FPacket(Data, Recipient)))
	{
		WorkEvent->Trigger();

		return true;
	}

	return false;
}





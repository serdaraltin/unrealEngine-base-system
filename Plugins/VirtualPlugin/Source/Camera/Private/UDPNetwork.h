// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "StreamSettings.h"
#include "Networking.h"

class StreamCommunication;
/**
 * 
 */
class UDPNetwork
{
public:
	UDPNetwork();
	~UDPNetwork();
};

//================================================================================================

class FStreamUDPReceiver
	: public FRunnable
{
	DECLARE_DELEGATE_TwoParams(FOnReceivedSignature, const FArrayReaderPtr&, const FIPv4Endpoint&);

public:

	~FStreamUDPReceiver();

	FOnReceivedSignature& OnReceived() { return OnReceivedDelegate; }

	virtual bool Init() override;
	virtual uint32 Run();

private:

	FRunnableThread* Thread;

protected:
	
	bool bIsStopping;
	FOnReceivedSignature OnReceivedDelegate;

public:
	FStreamUDPReceiver(FSocket* InSocket, const TCHAR* ThreadDescription);

	virtual void Stop() override;

protected:
	FSocket* Socket;
	FTimespan WaitTime;
};

//=========================================================================

class FStreamUDPSender
	: public FRunnable
{
public:

	virtual bool Init() override
	{
		return true;
	}


	uint32 GetThroughput() const
	{
		return Throughput;
	}

	bool Send(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Recipient);

	void SetSendRate(uint32 Rate)
	{
		SendRate = Rate;
	}

private:


	uint32 SendRate;

	bool Stopping;

	FRunnableThread* Thread;


public:

	uint32 GetSendRate() const
	{
		return SendRate;
	}


	void SetWaitTime(const FTimespan& Timespan)
	{
		WaitTime = Timespan;
	}


	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override { }

private:

	FTimespan WaitTime;

	FEvent* WorkEvent;

	struct FPacket
	{

		TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> Data;


		FIPv4Endpoint Recipient;

		FPacket() { }

		FPacket(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& InData, const FIPv4Endpoint& InRecipient)
			: Data(InData)
			, Recipient(InRecipient)
		{ }
	};

	uint32 Throughput;

	TQueue<FPacket, EQueueMode::Mpsc> SendQueue;

protected:
	FSocket* Socket;

public:

	FStreamUDPSender(FSocket* InSocket, const TCHAR* ThreadDescription);

	~FStreamUDPSender();
};


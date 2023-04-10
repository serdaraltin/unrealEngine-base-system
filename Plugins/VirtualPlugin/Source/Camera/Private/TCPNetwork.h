// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "StreamSettings.h"
#include "Networking.h"

class StreamCommunication;
/**
 * 
 */
class TCPNetwork
{
public:
	TCPNetwork();
	~TCPNetwork();
};

//===============================================================================================

class FStreamTCPConnector
	: public FRunnable
{

private:
	FRunnableThread* Thread;

public:
	DECLARE_DELEGATE_OneParam(FOnConnectSignature, bool);

	FStreamTCPConnector(StreamCommunication* InStreaming, FSocket* InSocket, TSharedRef<FInternetAddr> InAddr);
	~FStreamTCPConnector();

	FOnConnectSignature& OnConnect() { return OnConnectDelegate; }

	void Start();

protected:
	TSharedPtr<StreamCommunication> Streaming;
	FOnConnectSignature OnConnectDelegate;
	FDelegateHandle OnConnectHandle;

public:
	virtual bool Init() override;
	virtual uint32 Run();
	virtual void Stop() override;

protected:
	FSocket* Socket;
	TSharedRef<FInternetAddr> Addr;
	FTimespan WaitTime;
};

//===============================================================================================

class FStreamTCPReceiver
	: public FRunnable
{
	DECLARE_DELEGATE_OneParam(FOnReceivedSignature, const FArrayReaderPtr&);
private:

	FRunnableThread* Thread;


protected:
	TSharedPtr<StreamCommunication> Streaming;
	
	FOnReceivedSignature OnReceivedDelegate;

public:
	FStreamTCPReceiver(StreamCommunication* InStreaming, FSocket* InSocket, const TCHAR* ThreadDescription);

	virtual void Stop() override;

protected:
	FSocket* Socket;
	FTimespan WaitTime;
	bool bIsStopping;

public:

	~FStreamTCPReceiver();

	FOnReceivedSignature& OnReceived() { return OnReceivedDelegate; }

	virtual bool Init() override;
	virtual uint32 Run();

};

//===============================================================================================

class FStreamTCPSender
	: public FRunnable
{
private:

	uint32 SendRate;

	bool Stopping;

	FRunnableThread* Thread;

	uint32 Throughput;

protected:
	TSharedPtr<StreamCommunication> Streaming;
	FSocket* Socket;

public:

	FStreamTCPSender(StreamCommunication* InStreaming, FSocket* InSocket, const TCHAR* ThreadDescription);


	uint32 GetThroughput() const
	{
		return Throughput;
	}

	bool Send(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& Data);

	void SetSendRate(uint32 Rate)
	{
		SendRate = Rate;
	}

	~FStreamTCPSender();

public:

	virtual bool Init() override
	{
		return true;
	}

	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override { }

private:

	struct FPacket
	{
		TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> Data;

		FPacket() { }

		FPacket(const TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>& InData)
			: Data(InData)
		{ }
	};

	TQueue<FPacket, EQueueMode::Mpsc> SendQueue;

	

	FTimespan WaitTime;

	FEvent* WorkEvent;

public:

	uint32 GetSendRate() const
	{
		return SendRate;
	}

	void SetWaitTime(const FTimespan& Timespan)
	{
		WaitTime = Timespan;
	}

};

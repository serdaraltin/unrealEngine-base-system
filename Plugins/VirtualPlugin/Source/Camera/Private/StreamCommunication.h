// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "StreamSettings.h"
#include "Networking.h"
#include "TCPNetwork.h"
#include "UDPNetwork.h"



class VideoStreaming;
/**
 * 
 */
class StreamCommunication
{
private:
	FStreamTCPReceiver* TCPReceiver;
	uint8 ControllerId;

	double LastPingTime;

	FSocket* Socket;
	FIPv4Endpoint RemoteEndPoint;
protected:

	FBufferArchive Buffer;
	TSharedPtr<VideoStreaming> Streaming;
	bool bIsTCP;

public:
	StreamCommunication(VideoStreaming* InStreaming, bool bInTCP, const FString& InIp, int32 InPort, const FString& DebugDescription = TEXT(""));

	bool IsConnected()const { return bIsConnected; }

	void Pong();

	~StreamCommunication();

	void SetControllerId(int32 InControllerId);

	void OnReceived(bool bInIsTCP, const FArrayReaderPtr& Data);

	void OnAction(uint32 Action, const TArray<uint8>& Data);

protected:
	int32 Port;
	bool bIsConnected;

	FString Ip;

private:
	FIPv4Address RemoteAdress;

	FStreamTCPConnector* TCPConnector;
	FStreamTCPSender* TCPSender;
	double LastSetControllerTime;

public:


	void SendAction(uint32 InAction);
	uint8 GetControllerId()const { return ControllerId; }

	FIPv4Endpoint GetRemoteEndPoint()const { return RemoteEndPoint; }

	void Sync();

	void ForceClientControllerId();

	bool IsUdp()const { return !bIsTCP; }

	void Ping();

	void Stop();

	void OnTCPConnect(bool bWasSuccessful);

	void SendAction(uint32 InAction, const FBufferArchive& InData);


};

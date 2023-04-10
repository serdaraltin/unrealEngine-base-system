// Copyright 2020 ByOwls. All Rights Reserved.
#include "StreamCommunication.h"
#include "VideoStreaming.h"
#include "StreamCommunication.h"
#include "StreamSettings.h"
#include "ImageCompresser.h"
#include "IImageWrapper.h"

void StreamCommunication::SendAction(uint32 InAction, const FBufferArchive& InData)
{
	if (bIsConnected)
	{
		int32 DatLen = InData.Num();

		FBufferArchive Buf;
		Buf << InAction;
		Buf << DatLen;
		Buf.Append(InData.GetData(), InData.Num());

		TArray<uint8>* FinalData = new TArray<uint8>();
		FinalData->AddUninitialized(Buf.Num());
		FMemory::Memcpy(FinalData->GetData(), Buf.GetData(), Buf.Num());

		if (bIsTCP && TCPSender)
		{
			TCPSender->Send(MakeShareable(FinalData));
		}
		else if (Streaming->GetUDPSender())
		{
			Streaming->GetUDPSender()->Send(MakeShareable(FinalData), RemoteEndPoint);
		}
	}
}



void StreamCommunication::SendAction(uint32 InAction)
{
	if (bIsConnected)
	{
		int32 DatLen = 0;

		FBufferArchive Buf;
		Buf << InAction;
		Buf << DatLen;

		TArray<uint8>* FinalData = new TArray<uint8>();
		FinalData->AddUninitialized(Buf.Num());
		FMemory::Memcpy(FinalData->GetData(), Buf.GetData(), Buf.Num());

		if (bIsTCP && TCPSender)
		{
			TCPSender->Send(MakeShareable(FinalData));
		}
		else if (Streaming->GetUDPSender())
		{
			Streaming->GetUDPSender()->Send(MakeShareable(FinalData), RemoteEndPoint);
		}
	}
}

void StreamCommunication::ForceClientControllerId()
{
	LastSetControllerTime = FApp::GetCurrentTime();
}

void StreamCommunication::SetControllerId(int32 InControllerId)
{
	ControllerId = InControllerId;
	ForceClientControllerId();
	Sync();
}

void StreamCommunication::Sync()
{
	FBufferArchive Data;
	Data << ControllerId;

	SendAction(1, Data);
}


StreamCommunication::StreamCommunication(VideoStreaming* InStreaming, bool bInTCP, const FString& InIp, int32 InPort, const FString& DebugDescription)
	:Streaming(InStreaming)
{
	TCPConnector = nullptr;
	TCPReceiver = nullptr;
	TCPSender = nullptr;
	ControllerId = 0;
	LastPingTime = 0.f;
	LastSetControllerTime = 0.f;
	bIsTCP = bInTCP;
	Ip = InIp;
	Port = InPort;
	Socket = nullptr;
	bIsConnected = false;

	auto SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	FIPv4Address::Parse(Ip, RemoteAdress);
	RemoteEndPoint = FIPv4Endpoint(RemoteAdress, Port);

	if (bIsTCP)
	{
		Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("Video Stream"));
		TCPConnector = new FStreamTCPConnector(this, Socket, RemoteEndPoint.ToInternetAddr());
		TCPConnector->OnConnect().BindRaw(this, &StreamCommunication::OnTCPConnect);
		TCPConnector->Start();
	}
	else
	{
		bIsConnected = true;
	}
	ForceClientControllerId();
}


void StreamCommunication::Ping()
{
	const double CurrentTime = FApp::GetCurrentTime();
	if ((CurrentTime - LastPingTime) >= 0.1f)
	{
		LastPingTime = FApp::GetCurrentTime();
		SendAction(5);
	}
}

void StreamCommunication::OnReceived(bool bInIsTCP, const FArrayReaderPtr& Data)
{
	if (!bInIsTCP)
	{
		Buffer.Empty();
	}

	if (Data->Num() > 0)
	{
		Buffer.Append(Data->GetData(), Data->Num());
	}

	if (Buffer.Num() >= 9)
	{
		bool bNext = false;
		do {
			bNext = false;

			FBufferReader Input(Buffer.GetData(), Buffer.Num(), false);

			uint8 NewControllerId = 0;
			int32 Action = 0;
			int32 DatLen = 0;

			Input << NewControllerId;
			Input << Action;
			Input << DatLen;

			if ((FApp::GetCurrentTime() - LastSetControllerTime) > 2.f)
			{
				if (ControllerId != NewControllerId)
				{
					int32 OldControllerId = ControllerId;
					ControllerId = NewControllerId;
					Streaming->OnControllerChanged(this, OldControllerId, NewControllerId);
				}
			}

			if ((Input.TotalSize() - Input.Tell()) >= DatLen)
			{
				if (DatLen > 0)
				{
					// Prev seek. So reader can read Array!
					Input.Seek(Input.Tell() - 4);

					TArray<uint8> DataBuf;
					Input << DataBuf;

					OnAction(Action, DataBuf);
				}
				else
				{
					OnAction(Action, TArray<uint8>());
				}

				if ((Input.TotalSize() - Input.Tell()) >= 9)
				{
					bNext = true;
				}

				// Slice buffer
				Buffer.RemoveAt(0, Input.Tell());

			}
		} while (bNext);
	}
}

void StreamCommunication::Pong()
{
	SendAction(6);
}

void StreamCommunication::OnTCPConnect(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		bIsConnected = true;

		ForceClientControllerId();

		TCPSender = new FStreamTCPSender(this, Socket, TEXT("Video Stream - TCP Sender"));
		TCPReceiver = new FStreamTCPReceiver(this, Socket, TEXT("Video Stream - TCP Receiver"));
		TCPReceiver->OnReceived().BindLambda([&](const FArrayReaderPtr& Data) {
			OnReceived(true, Data);
		});
	}
}


void StreamCommunication::OnAction(uint32 Action, const TArray<uint8>& Data)
{
	if (Action == 5)
	{
		Pong();
	}
	else
	{
		Streaming->OnActionReceived(ControllerId, Action, Data);
	}
}

void StreamCommunication::Stop()
{
	if (bIsConnected)
	{
		// Send close signal!
		SendAction(0);

		// @todo: Close the socket before the call ->Stop functions
		if (Socket)
		{
			Socket->Close();
		}

		if (TCPConnector)
		{
			TCPConnector->Stop();
		}

		if (TCPSender)
		{
			TCPSender->Stop();
		}
		if (TCPReceiver)
		{
			TCPReceiver->Stop();
		}
	}
}

StreamCommunication::~StreamCommunication()
{
	if (Socket != nullptr)
	{
		Socket->Close();
		auto SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}
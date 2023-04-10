// Copyright 2020 ByOwls. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "ImageCompresser.h"

class FStreamUDPReceiver;
class FStreamUDPSender;

class VideoStreaming
{
public:

	void SendVideoFrameBuffer(int32 ControllerId, const uint8* VideoFrameBuffer, uint32 BufferSize, int32 Width, int32 Height);

	FVector2D ConvertTouchToScreen(int32 ControllerID, const FVector2D TouchPosition);

	struct FTouchInfo
	{
		int32 ControllerId;
		int32 TouchId;
		FVector2D Location;

		FTouchInfo()
			: ControllerId(0), TouchId(0), Location(FVector::ZeroVector) {}

		FTouchInfo(int32 InControllerId, int32 InTouchId)
			: ControllerId(InControllerId), TouchId(InTouchId), Location(FVector::ZeroVector) {}

		FTouchInfo(int32 InControllerId, int32 InTouchId, const FVector2D& InLocation)
			: ControllerId(InControllerId), TouchId(InTouchId), Location(InLocation) {}
	};

	bool IsBroadcasting() const { return bIsBroadcasting; }
	void StopStreaming();

	bool FindViewportGeometryInternal(const FGeometry& Geometry, TSharedPtr<SWidget> Widget, FGeometry& OutGeometry)const;

	struct FViewportInfo
	{
		FVector2D Position;
		FVector2D Size;

		FViewportInfo()
			: Position(FVector2D::ZeroVector), Size(FVector2D::ZeroVector) {}
	};

	class StreamCommunication* GetCommunication(int32 ControllerId) const;
	uint8 GetOrientation(int32 ControllerId = 0) const;

	FORCEINLINE FStreamUDPSender* GetUDPSender()const { return UDPSender; }

	FViewportInfo GetViewportInfo(int32 ControllerId = 0)const;

	FString GetADBPath()const;

	void ShutdownModule();

	void StartUDP();

	void OpenADBConnection();

	TMap<int32, double> LastBroadcastTimes;
public:

	bool HasTouchInfo(int32 ControllerID, int32 TouchID)const;

	void StartCopyingNextVideoFrame(int32  ControllerId, const FViewportRHIRef& ViewportRHI);

	void UpdateLastTouchInfo(int32 ControllerID, int32 TouchID, FVector2D TouchLocation);

	void OnTouchStarted(int32 ControllerId, int32 TouchId, FVector2D TouchLocation);
	void OnTouchMoved(int32 ControllerId, int32 TouchId, FVector2D TouchLocation);

	bool GetLastTouchInfo(int32 ControllerID, int32 TouchID, FTouchInfo& OutTouchInfo)const;

	struct FReceivedAction
	{
		int32 ControllerId;
		int32 Action;
		TArray<uint8> Data;

		FReceivedAction(int32 InControllerId, int32 InAction, const TArray<uint8> InData)
			: ControllerId(InControllerId), Action(InAction), Data(InData) {}
	};

	void CloseADBConnection();

	void OnSlateRendered(SWindow& SlateWindow, void* ViewportRHIPtr);

	void BroadcastVideoFrame(int32 ControllerId);

	void StartVideoStream(FString Ip, class AMyActor* OurActor);

	bool Tick(float DeltaSeconds);

	struct FPlayerReadback
	{
		FIntPoint Resolution;
		uint32 BufferSize;

		void* ReadbackBuffer;

		FPlayerReadback(FVector2D InResolution)
			: Resolution(FIntPoint(InResolution.X, InResolution.Y)), bIsBufferReady(false)
		{
			BufferSize = Resolution.X * Resolution.Y * 4;
			ReadbackBuffer = new uint8[BufferSize];
		}

		void ReleaseResources()
		{
			delete[](uint8*)ReadbackBuffer;
			ReadbackBuffer = nullptr;
		}

		FORCEINLINE bool IsBufferReady() { return bIsBufferReady; }

		void SetBufferReady(bool bValue) { bIsBufferReady = bValue; }

		FORCEINLINE uint32 GetBufferSize()const { return BufferSize; }

		void*& GetBuffer() { return ReadbackBuffer; }

		void Unmap() { bIsBufferReady = false; }

	private:
		bool bIsBufferReady;
	};
	TMap<int32, TSharedPtr<FPlayerReadback, ESPMode::ThreadSafe>> PlayerReadbacks;

	void OnTouchActionReceived(const FReceivedAction& ReceivedAction);

	bool FindViewportGeometry(TSharedPtr<SWindow> Window, FGeometry& OutGeometry)const;

	void OnTouchEnded(int32 ControllerId, int32 TouchId, FVector2D TouchLocation);
private:

	bool bPendingStart;
	float StartTimeCounter;

	TArray<StreamCommunication*> Communications;

	TArray<FAsyncTask<ImageCompresser>*> Actions;

	FCriticalSection SyncActionsSection;

	FTickerDelegate TickDelegate;

	FStreamUDPSender* UDPSender;

public:


	void SendAction(int32 ControllerId, uint32 InAction);
	void SendAction(int32 ControllerId, uint32 InAction, const FBufferArchive& InData);

	void OnControllerChanged(class StreamCommunication* Comm, int32 OldControllerId, int32 NewControllerId);
	void SyncActions();

	void OnActionReceived(int32 ControllerId, int32 Action, const TArray<uint8> Data);


	struct FPlayerInfo
	{
		UGameViewportClient* ViewportClient;
		APlayerController* PlayerController;
		FViewportInfo ViewportInfo;

		FPlayerInfo()
			: ViewportClient(nullptr), PlayerController(nullptr) {}
	};

	TMap<int32, FPlayerInfo> PlayerInfos;

	void InitCommunication(FString Ip = TEXT(""));

	void OnUDPReceived(const FArrayReaderPtr& Data, const FIPv4Endpoint& Addr);

	VideoStreaming();
	void StartupModule();

	void StopUDP();

private:

	class AMyActor* MyOurActor;

	FVector2D BroadcastResolution;

	TArray<FReceivedAction> ReceivedActions;

	FDelegateHandle TickDelegateHandle;

	FSocket* SocketUdp;
	FStreamUDPReceiver* UDPReceiver;

	FCriticalSection ReceivedActionsLock;

	TArray<FTouchInfo> LastTouchs;

	bool bIsBroadcasting;

};

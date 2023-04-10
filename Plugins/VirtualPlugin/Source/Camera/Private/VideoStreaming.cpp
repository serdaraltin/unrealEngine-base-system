// Copyright 2020 ByOwls. All Rights Reserved.

#include "VideoStreaming.h"
#include "Engine.h"
#include "StreamCommunication.h"
#include "StreamSettings.h"
#include "ImageCompresser.h"
#include "IImageWrapper.h"
#include "MyActor.h"

#include "Runtime/Core/Public/Features/IModularFeatures.h"
#include "SlateBasics.h"
#include "ScreenRendering.h"
#include "RenderCore.h"
#include "RHIStaticStates.h"
#include "MediaShaders.h"
#include "RendererInterface.h"
#include "Framework/Application/SlateApplication.h"
#include "PipelineStateCache.h"
#include "Runtime/Launch/Resources/Version.h"
#include "CommonRenderResources.h"
#include "RenderTargetPool.h"

#include "RHI.h"

#define LOCTEXT_NAMESPACE "VideoStream"


void VideoStreaming::StartCopyingNextVideoFrame(int32 ControllerId, const FViewportRHIRef& ViewportRHI)
{
	static const FName RenderModuleName("Renderer");
	IRendererModule& RenderModule = FModuleManager::GetModuleChecked<IRendererModule>(RenderModuleName);

	UGameViewportClient* GameViewportClient = GEngine->GameViewport;
	check(GameViewportClient != nullptr);

	if (PlayerInfos.Contains(ControllerId))
	{
		auto PlayerInfo = PlayerInfos[ControllerId];

		auto PlayerReadback = PlayerReadbacks[ControllerId];
		if (!PlayerReadback.IsValid())
		{
			return;
		}

		FVector2D UVSize;
		FVector2D UV;
		FVector2D WindowSize;

		if (PlayerInfo.ViewportInfo.Size.X < 850 || PlayerInfo.ViewportInfo.Size.Y < 550) {

			WindowSize = PlayerInfo.ViewportClient->GetWindow()->GetSizeInScreen();
			UV = PlayerInfo.ViewportInfo.Position / PlayerInfo.ViewportClient->GetWindow()->GetSizeInScreen();


			UVSize.X = (1 - (PlayerInfo.ViewportInfo.Size.Y / PlayerInfo.ViewportClient->GetWindow()->GetSizeInScreen().Y));
			UVSize.Y = (1 - (PlayerInfo.ViewportInfo.Size.X / PlayerInfo.ViewportClient->GetWindow()->GetSizeInScreen().X));
		}
		else {

			WindowSize = PlayerInfo.ViewportInfo.Size;
			UV = PlayerInfo.ViewportInfo.Position / PlayerInfo.ViewportClient->GetWindow()->GetSizeInScreen();

			UVSize.X = 1.f - UV.X;
			UVSize.Y = 1.f - UV.Y;
		}

		struct FCopyVideoFrame
		{
			FViewportRHIRef ViewportRHI;
			IRendererModule* RenderModule;
			FIntPoint Resolution;
			FVector2D UV;
			FVector2D UVSize;
			TSharedPtr<FPlayerReadback, ESPMode::ThreadSafe> PlayerReadback;
			VideoStreaming* This;
		};
		FCopyVideoFrame CopyVideoFrame =
		{
			ViewportRHI,
			&RenderModule,
			FIntPoint(BroadcastResolution.X, BroadcastResolution.Y),
			UV,
			UVSize,
			PlayerReadback,
			this
		};

		FCopyVideoFrame Context = CopyVideoFrame;
		ENQUEUE_RENDER_COMMAND(ReadSurfaceCommand)(
			[Context](FRHICommandListImmediate& RHICmdList)
		{
			FPooledRenderTargetDesc OutputDesc(FPooledRenderTargetDesc::Create2DDesc(Context.Resolution, PF_R8G8B8A8, FClearValueBinding::None, TexCreate_None, TexCreate_RenderTargetable, false));

			const auto FeatureLevel = GMaxRHIFeatureLevel;

			TRefCountPtr<IPooledRenderTarget> ResampleTexturePooledRenderTarget;
			GRenderTargetPool.FindFreeElement(RHICmdList, OutputDesc, ResampleTexturePooledRenderTarget, TEXT("ResampleTexture"));
			check(ResampleTexturePooledRenderTarget);

			const FSceneRenderTargetItem& DestRenderTarget = ResampleTexturePooledRenderTarget->GetRenderTargetItem();

			FRHIRenderPassInfo RPInfo(DestRenderTarget.TargetableTexture, ERenderTargetActions::DontLoad_Store);
			RHICmdList.BeginRenderPass(RPInfo, TEXT("VideoStream"));

			RHICmdList.SetViewport(0, 0, 0.0f, Context.Resolution.X, Context.Resolution.Y, 1.0f);
			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

			GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGB>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

			FTexture2DRHIRef ViewportBackBuffer = RHICmdList.GetViewportBackBuffer(Context.ViewportRHI);

			//Doesnt work in 4.25
			//auto ShaderMap = GetGlobalShaderMap(FeatureLevel);
			//TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
			//TShaderMapRef<FScreenPS> PixelShader(ShaderMap);
			//REplaced with:
			TShaderMapRef<FScreenVS> VertexShader(GetGlobalShaderMap(FeatureLevel));
			TShaderMapRef<FScreenPS> PixelShader(GetGlobalShaderMap(FeatureLevel));

			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
			//Doesnt work in 4.25
			//GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
			//GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
			//REplaced with:
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();

			GraphicsPSOInit.PrimitiveType = PT_TriangleList;

			SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

			if (Context.Resolution != FIntPoint(ViewportBackBuffer->GetSizeX(), ViewportBackBuffer->GetSizeY()))
			{
				PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), ViewportBackBuffer);
			}
			else
			{
				PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Point>::GetRHI(), ViewportBackBuffer);
			}

			Context.RenderModule->DrawRectangle(
				RHICmdList,
				0, 0,
				Context.Resolution.X, Context.Resolution.Y,
				Context.UV.X, Context.UV.Y,
				Context.UVSize.X, Context.UVSize.Y,
				Context.Resolution,
				FIntPoint(1, 1),
				VertexShader,
				EDRF_Default);
			RHICmdList.EndRenderPass();

			FIntRect Rect = FIntRect(0, 0, Context.Resolution.X, Context.Resolution.Y);

			TArray<FColor> OutData;
			RHICmdList.ReadSurfaceData(
				DestRenderTarget.TargetableTexture,
				Rect,
				OutData,
				FReadSurfaceDataFlags()
			);
			for (int32 Index = 0; Index < OutData.Num(); Index++)
			{
				auto Tmp = OutData[Index].B;
				OutData[Index].B = OutData[Index].R;
				OutData[Index].R = Tmp;
			}

			FMemory::Memcpy(Context.PlayerReadback->GetBuffer(), OutData.GetData(), Context.PlayerReadback->GetBufferSize());
			Context.PlayerReadback->SetBufferReady(true);
		});
	}
}


void VideoStreaming::OnControllerChanged(StreamCommunication* Comm, int32 OldControllerId, int32 NewControllerId)
{
	bool bReplaced = false;

	for (int32 Index = 0; Index < Communications.Num(); Index++)
	{
		auto Item = Communications[Index];
		if (Item != Comm)
		{
			if (Item->GetControllerId() == NewControllerId)
			{
				if (!bReplaced)
				{
					Item->SetControllerId(OldControllerId);
					bReplaced = true;
				}
				else
				{
					Item->SetControllerId(0);
				}
			}
		}
	}
}

bool VideoStreaming::FindViewportGeometry(TSharedPtr<SWindow> Window, FGeometry& OutGeometry)const
{
	if (Window.IsValid())
	{
		return FindViewportGeometryInternal(Window->GetWindowGeometryInWindow(), Window, OutGeometry);
	}

	return false;
}

void VideoStreaming::StopUDP()
{
	if (UDPSender)
	{
		UDPSender->Stop();
		UDPSender = nullptr;
	}

	if (UDPReceiver)
	{
		UDPReceiver->Stop();
		UDPReceiver = nullptr;
	}

	if (SocketUdp)
	{
		SocketUdp->Close();
	}

	if (SocketUdp)
	{
		auto SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		if (SocketSubsystem)
		{
			SocketSubsystem->DestroySocket(SocketUdp);
		}
		SocketUdp = nullptr;
	}
}



StreamCommunication* VideoStreaming::GetCommunication(int32 ControllerId) const
{
	if (Communications.Num() > 1)
	{
		for (int32 Index = 0; Index < Communications.Num(); Index++)
		{
			if (Communications[Index]->GetControllerId() == ControllerId)
			{
				return Communications[Index];
			}
		}
	}
	else if (Communications.Num() > 0)
	{
		return Communications[0];
	}

	return nullptr;
}

void VideoStreaming::SyncActions()
{
	FScopeLock Lock(&SyncActionsSection);

	for (int32 Index = 0; Index < Actions.Num(); Index++)
	{
		auto Action = Actions[Index];
		if (Action)
		{
			Action->EnsureCompletion(true);
			delete Action;
		}

		Actions.RemoveAt(Index);
		--Index;
	}
}


void VideoStreaming::OnTouchStarted(int32 ControllerId, int32 TouchId, FVector2D TouchLocation)
{
	if (PlayerInfos.Contains(ControllerId))
	{
		auto PlayerInfo = PlayerInfos[ControllerId];

		const FVector2D TouchScreen = ConvertTouchToScreen(ControllerId, TouchLocation);

		FPointerEvent PointerEvent(
			ControllerId,
			TouchId,
			TouchScreen,
			TouchScreen,
			1.f,
			true);

		if (!FSlateApplication::Get().ProcessMouseButtonDownEvent(PlayerInfo.ViewportClient->GetWindow()->GetNativeWindow(), PointerEvent))
		{
			if (PlayerInfo.PlayerController)
			{
				PlayerInfo.PlayerController->InputTouch(TouchId, ETouchType::Began, TouchLocation, 1.f, FDateTime::Now(), 0);
			}
		}
		if (TouchId == 0 && PlayerInfos.Num() == 0 && FSlateApplication::Get().IsDragDropping())
		{
			FSlateApplication::Get().SetCursorPos(TouchScreen);
		}
	}
}


void VideoStreaming::OnActionReceived(int32 ControllerId, int32 Action, const TArray<uint8> Data)
{
	FScopeLock Lock(&ReceivedActionsLock);
	ReceivedActions.Add(FReceivedAction(Communications.Num() > 1 ? ControllerId : 0, Action, Data));
}

FString VideoStreaming::GetADBPath()const
{
#if !UE_BUILD_SHIPPING
	FString AndroidDirectory = FPlatformMisc::GetEnvironmentVariable(TEXT("ANDROID_HOME"));

	FString ADBPath = TEXT("");
	if (AndroidDirectory.Len() > 0)
	{
#if PLATFORM_WINDOWS
		ADBPath = FString::Printf(TEXT("%s\\platform-tools\\adb.exe"), *AndroidDirectory);
#else
		ADBPath = FString::Printf(TEXT("%s/platform-tools/adb"), *AndroidDirectory);
#endif
	}

	return ADBPath;
#else
	return TEXT("");
#endif
}

void VideoStreaming::OpenADBConnection()
{
#if !UE_BUILD_SHIPPING
	const auto& Settings = *MyOurActor->StreamSetting;

	FString ADBPath = GetADBPath();
	if (!ADBPath.IsEmpty() && FPaths::FileExists(*ADBPath))
	{
		const FString CommandLine = FString::Printf(TEXT("forward tcp:%i tcp:%i"), Settings.Port, Settings.Port);;

		int32 ReturnCode;
		FString OutStdOut;
		FString OutStdErr;
		FPlatformProcess::ExecProcess(*ADBPath, *CommandLine, &ReturnCode, &OutStdOut, &OutStdErr);
	}
#endif
}

void VideoStreaming::CloseADBConnection()
{
#if !UE_BUILD_SHIPPING
	const auto& Settings = *MyOurActor->StreamSetting;

	FString ADBPath = GetADBPath();
	if (!ADBPath.IsEmpty() && FPaths::FileExists(*ADBPath))
	{
		const FString CommandLine = FString::Printf(TEXT("forward --remove tcp:%i"), Settings.Port);

		int32 ReturnCode;
		FString OutStdOut;
		FString OutStdErr;
		FPlatformProcess::ExecProcess(*ADBPath, *CommandLine, &ReturnCode, &OutStdOut, &OutStdErr);

	}
#endif
}

void VideoStreaming::SendAction(int32 ControllerId, uint32 InAction)
{
	class StreamCommunication* const Comm = GetCommunication(ControllerId);
	if (Comm && Comm->IsConnected())
	{
		Comm->SendAction(InAction);
	}
}

void VideoStreaming::SendAction(int32 ControllerId, uint32 InAction, const FBufferArchive& InData)
{
	class StreamCommunication* const Comm = GetCommunication(ControllerId);
	if (Comm && Comm->IsConnected())
	{
		Comm->SendAction(InAction, InData);

	}
}



void VideoStreaming::StartUDP()
{
	StopUDP();

	const auto& Settings = *MyOurActor->StreamSetting;

	auto SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	SocketUdp = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("Video Stream"), true);
	check(SocketUdp != nullptr);

	SocketUdp->SetNonBlocking(true);
	SocketUdp->SetReuseAddr(true);
	SocketUdp->SetBroadcast(false);

	int32 BufferSize = 65535;
	int32 OutNewSize;
	SocketUdp->SetReceiveBufferSize(BufferSize, OutNewSize);
	SocketUdp->SetSendBufferSize(BufferSize, OutNewSize);

	TSharedPtr<FInternetAddr> MyAddr = SocketSubsystem->CreateInternetAddr();
	MyAddr->SetAnyAddress();
	MyAddr->SetPort(Settings.Port);
	SocketUdp->Bind(*MyAddr);

	UDPReceiver = new FStreamUDPReceiver(SocketUdp, TEXT("Video Stream - UDP Receiver"));
	UDPReceiver->OnReceived().BindRaw(this, &VideoStreaming::OnUDPReceived);

	UDPSender = new FStreamUDPSender(SocketUdp, TEXT("Video Stream - UDP Sender"));
}



bool VideoStreaming::FindViewportGeometryInternal(const FGeometry& Geometry, TSharedPtr<SWidget> Widget, FGeometry& OutGeometry)const
{
	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	Widget->ArrangeChildren(Geometry, ArrangedChildren);
	for (int32 Index = 0; Index < ArrangedChildren.Num(); ++Index)
	{
		TSharedPtr<SWidget> ChildWidget = ArrangedChildren[Index].Widget;
		FGeometry ChildGeometry = ArrangedChildren[Index].Geometry;
		static FName NAME_Viewport(TEXT("SGameLayerManager"));
		if (ChildWidget->GetType() == NAME_Viewport)
		{
			OutGeometry = ArrangedChildren[Index].Geometry;
			return true;
		}
		else
		{
			const bool bResult = FindViewportGeometryInternal(ChildGeometry, ChildWidget, OutGeometry);
			if (bResult)
			{
				return true;
			}
		}
	}

	return false;
}

VideoStreaming::FViewportInfo VideoStreaming::GetViewportInfo(int32 ControllerId)const
{
	if (PlayerInfos.Contains(ControllerId))
	{
		return PlayerInfos[ControllerId].ViewportInfo;
	}

	return FViewportInfo();
}

void VideoStreaming::SendVideoFrameBuffer(int32 ControllerId, const uint8* VideoFrameBuffer, uint32 BufferSize, int32 Width, int32 Height)
{
	uint8* BufferData = new uint8[BufferSize];
	FMemory::Memcpy(BufferData, VideoFrameBuffer, BufferSize);

	SyncActions();

	const auto& Settings = *MyOurActor->StreamSetting;
	auto Task = new FAsyncTask<class ImageCompresser>(this, ControllerId, BufferData, BufferSize, Width, Height, Settings.GetRealQuality());
	Actions.Add(Task);

	Task->StartBackgroundTask();
}


uint8 VideoStreaming::GetOrientation(int32 ControllerId)const
{
	FViewportInfo Info = GetViewportInfo(ControllerId);
	return Info.Size.X > Info.Size.Y ? 2 : 1;
}


VideoStreaming::VideoStreaming()
{
	UDPReceiver = nullptr;
	UDPSender = nullptr;
	MyOurActor = nullptr;
	bIsBroadcasting = false;
	BroadcastResolution = FVector2D(512, 512);
	SocketUdp = nullptr;
	bPendingStart = false;
	StartTimeCounter = 0.f;
}

void VideoStreaming::StartupModule()
{
	TickDelegate = FTickerDelegate::CreateRaw(this, &VideoStreaming::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void VideoStreaming::ShutdownModule()
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	StopStreaming();
}


void VideoStreaming::OnTouchActionReceived(const FReceivedAction& ReceivedAction)
{
	if (!PlayerInfos.Contains(ReceivedAction.ControllerId))
	{
		return;
	}

	const auto& PlayerInfo = PlayerInfos[ReceivedAction.ControllerId];
	class UGameViewportClient* const ViewportClient = PlayerInfo.ViewportClient;

	uint8_t ControllerId = ReceivedAction.ControllerId;

	TArray<FTouchInfo> ParsedTouchs;
	if (ReceivedAction.Data.Num() > 0)
	{
		FBufferReader Buffer((void*)ReceivedAction.Data.GetData(), ReceivedAction.Data.Num(), false);

		while ((Buffer.TotalSize() - Buffer.Tell()) >= 9)
		{
			uint8 TouchId = 0;
			float TouchX = 0;
			float TouchY = 0;

			Buffer << TouchId;
			Buffer << TouchX;
			Buffer << TouchY;

			ParsedTouchs.Add(FTouchInfo(ControllerId, TouchId));

			FViewportInfo ViewportInfo = GetViewportInfo(ControllerId);

			const FVector2D TouchGame = FVector2D(TouchX, TouchY);

			if (HasTouchInfo(ControllerId, TouchId))
			{
				OnTouchMoved(ControllerId, TouchId, TouchGame);
				UpdateLastTouchInfo(ControllerId, TouchId, TouchGame);
			}
			else
			{
				OnTouchStarted(ControllerId, TouchId, TouchGame);
				LastTouchs.Add(FTouchInfo(ControllerId, TouchId, (TouchGame)));
			}

		}
	}

	TArray<int32> Removed;
	for (int32 Index = 0; Index < LastTouchs.Num(); Index++)
	{
		const FTouchInfo& TouchInfo = LastTouchs[Index];
		int32 TouchId = TouchInfo.TouchId;


		bool bFound = false;
		for (int32 Index2 = 0; Index2 < ParsedTouchs.Num(); Index2++)
		{
			if (ParsedTouchs[Index2].ControllerId == ControllerId && ParsedTouchs[Index2].TouchId == TouchId)
			{
				bFound = true;
				break;
			}
		}

		if (TouchInfo.ControllerId == ControllerId && !bFound)
		{
			OnTouchEnded(ControllerId, TouchId, TouchInfo.Location);
			Removed.Add(Index);
		}
	}

	for (int32 Index = 0; Index < Removed.Num(); Index++)
	{
		LastTouchs.RemoveAt(Removed[Index]);
	}
}


void VideoStreaming::OnUDPReceived(const FArrayReaderPtr& Data, const FIPv4Endpoint& Addr)
{
	if (bIsBroadcasting)
	{
		for (int32 Index = 0; Index < Communications.Num(); Index++)
		{
			auto Comm = Communications[Index];
			if (Comm && Comm->IsUdp() && Comm->IsConnected() && Comm->GetRemoteEndPoint() == Addr)
			{
				Comm->OnReceived(false, Data);
			}
		}
	}
}

void VideoStreaming::InitCommunication(FString Ip)
{
	const auto& Settings = *(MyOurActor->StreamSetting);

	StreamCommunication* FirstCommunication = nullptr;

	if (Ip.IsEmpty())
	{
		FirstCommunication = (new StreamCommunication(this, false, Settings.IpAddress, Settings.Port));
		Communications.Add(FirstCommunication);
	}
	else
	{
		FirstCommunication = (new StreamCommunication(this, false, Ip, Settings.Port));
		Communications.Add(FirstCommunication);
	}
}

void VideoStreaming::StopStreaming()
{
	if (bIsBroadcasting)
	{
		bIsBroadcasting = false;

		for (int32 Index = 0; Index < Communications.Num(); Index++)
		{
			Communications[Index]->Stop();
		}
		Communications.Empty();

		StopUDP();

		if (FSlateApplication::IsInitialized())
		{
			FSlateRenderer* SlateRenderer = FSlateApplication::Get().GetRenderer();
			SlateRenderer->OnSlateWindowRendered().RemoveAll(this);
		}

		{
			FlushRenderingCommands();

			for (auto It = PlayerReadbacks.CreateConstIterator(); It; ++It)
			{
				auto PlayerReadback = It.Value();
				PlayerReadback->ReleaseResources();
			}

			PlayerReadbacks.Empty();
		}

		CloseADBConnection();
	}
}


FVector2D VideoStreaming::ConvertTouchToScreen(int32 ControllerID, const FVector2D TouchPosition)
{
	if (PlayerInfos.Contains(ControllerID))
	{
		const auto& PlayerInfo = PlayerInfos[ControllerID];
		FVector2D ScreenPosition;

		FSlateApplication& SlateApplication = FSlateApplication::Get();

		TSharedPtr<SViewport> GameViewport = PlayerInfo.ViewportClient->GetGameViewportWidget();
		if (GameViewport.IsValid())
		{
			FWidgetPath WidgetPath;

			FWeakWidgetPath GameViewportWidgetPath;

			WidgetPath = GameViewportWidgetPath.ToWidgetPath();
			SlateApplication.FindPathToWidget(GameViewport.ToSharedRef(), WidgetPath);
			GameViewportWidgetPath = WidgetPath;

			const FGeometry& GameViewportGeometry = WidgetPath.Widgets.Last().Geometry;
			ScreenPosition = GameViewportGeometry.LocalToAbsolute(TouchPosition * GameViewportGeometry.Size);
		}
		else
		{
			const FSlateRect WindowScreenRect = SlateApplication.GetActiveTopLevelWindow()->GetRectInScreen();
			const FVector2D WindowPosition = WindowScreenRect.GetSize() * TouchPosition;
			ScreenPosition = FVector2D(WindowScreenRect.Left, WindowScreenRect.Top) + WindowPosition;
		}

		return ScreenPosition;
	}

	return FVector2D::ZeroVector;
}

bool VideoStreaming::HasTouchInfo(int32 ControllerID, int32 TouchID)const
{
	for (const FTouchInfo& TouchInfo : LastTouchs)
	{
		if (TouchInfo.ControllerId == ControllerID && TouchInfo.TouchId == TouchID)
		{
			return true;
		}
	}

	return false;
}



bool VideoStreaming::Tick(float DeltaSeconds)
{
	class UGameViewportClient* const ViewportClient = GEngine->GameViewport;

#if !UE_BUILD_SHIPPING

	if (ViewportClient && ViewportClient->GetWorld()->HasBegunPlay() && ViewportClient->GetWorld()->RealTimeSeconds > 0.25f && !bIsBroadcasting && !bPendingStart)
	{
		bPendingStart = true;
		StartTimeCounter = 0.2f;
	}
	else if (!ViewportClient)
	{
		if (bIsBroadcasting)
		{
			StopStreaming();
		}
		bPendingStart = false;
	}

	if (bPendingStart)
	{
		StartTimeCounter -= DeltaSeconds;
		if (StartTimeCounter <= 0)
		{
			bPendingStart = false;
		}
	}
#endif
	if (bPendingStart || bIsBroadcasting)
	{
		TArray<APlayerController*> PlayerControllers;
		GEngine->GetAllLocalPlayerControllers(PlayerControllers);
		for (int32 Index = 0; Index < PlayerControllers.Num(); Index++)
		{
			int32 ControllerId = Index;

			class APlayerController* const PC = PlayerControllers[Index];
			if (PC && PC->GetLocalPlayer())
			{
				FPlayerInfo Info;
				Info.ViewportClient = PC->GetLocalPlayer()->ViewportClient;
				Info.PlayerController = PC;

				FGeometry ViewportGeometry;
				const bool bResult = FindViewportGeometry(Info.ViewportClient->GetWindow(), ViewportGeometry);
				if (bResult)
				{
					Info.ViewportInfo.Position = ViewportGeometry.LocalToAbsolute(FVector2D::ZeroVector);
					Info.ViewportInfo.Size = ViewportGeometry.GetLocalSize();
				}

				PlayerInfos.Add(ControllerId, Info);

				if (!PlayerReadbacks.Contains(ControllerId))
				{
					PlayerReadbacks.Add(ControllerId, MakeShareable(new FPlayerReadback(BroadcastResolution)));
				}
			}
		}
	}

	if (bIsBroadcasting && ViewportClient)
	{
		for (int32 Index = 0; Index < Communications.Num(); Index++)
		{
			Communications[Index]->Ping();
		}
		FScopeLock Lock(&ReceivedActionsLock);

		for (int32 Index = 0; Index < ReceivedActions.Num(); Index++)
		{
			const FReceivedAction& ReceivedAction = ReceivedActions[Index];
			if (ReceivedAction.Action == 3)
			{
				OnTouchActionReceived(ReceivedAction);
			}
		}

		ReceivedActions.Empty();
	}

	return true;
}

bool VideoStreaming::GetLastTouchInfo(int32 ControllerID, int32 TouchID, FTouchInfo& OutTouchInfo)const
{
	for (const FTouchInfo& TouchInfo : LastTouchs)
	{
		if (TouchInfo.ControllerId == ControllerID && TouchInfo.TouchId == TouchID)
		{
			OutTouchInfo = TouchInfo;
			return true;
		}
	}

	return false;
}

void VideoStreaming::UpdateLastTouchInfo(int32 ControllerID, int32 TouchID, FVector2D TouchLocation)
{
	for (FTouchInfo& TouchInfo : LastTouchs)
	{
		if (TouchInfo.ControllerId == ControllerID && TouchInfo.TouchId == TouchID)
		{
			TouchInfo.Location = TouchLocation;
			break;
		}
	}
}



void VideoStreaming::OnTouchEnded(int32 ControllerId, int32 TouchId, FVector2D TouchLocation)
{
	if (PlayerInfos.Contains(ControllerId))
	{
		auto PlayerInfo = PlayerInfos[ControllerId];

		const FVector2D TouchScreen = ConvertTouchToScreen(ControllerId, TouchLocation);

		FPointerEvent PointerEvent(
			ControllerId,
			TouchId,
			TouchScreen,
			TouchScreen,
			1.f,
			true);

		if (!FSlateApplication::Get().ProcessMouseButtonUpEvent(PointerEvent))
		{
			if (PlayerInfo.PlayerController)
			{
				PlayerInfo.PlayerController->InputTouch(TouchId, ETouchType::Ended, TouchLocation, 1.f, FDateTime::Now(), 0);
			}
		}
	}
}



void VideoStreaming::StartVideoStream(FString Ip, AMyActor* OurActor)
{

	MyOurActor = OurActor;

	if (!bIsBroadcasting)
	{
		PlayerInfos.Empty();

		PlayerReadbacks.Empty();

		OpenADBConnection();

		InitCommunication(Ip);

		StartUDP();

		UGameViewportClient* GameViewportClient = GEngine->GameViewport;
		check(GameViewportClient != nullptr);
		
		BroadcastResolution = FVector2D(MyOurActor->StreamSetting->Resolution, MyOurActor->StreamSetting->Resolution);

		FSlateRenderer* SlateRenderer = FSlateApplication::Get().GetRenderer();
		SlateRenderer->OnSlateWindowRendered().RemoveAll(this);
		SlateRenderer->OnSlateWindowRendered().AddRaw(this, &VideoStreaming::OnSlateRendered);

		bIsBroadcasting = true;
	}
}

void VideoStreaming::OnTouchMoved(int32 ControllerId, int32 TouchId, FVector2D TouchLocation)
{
	if (PlayerInfos.Contains(ControllerId))
	{
		auto PlayerInfo = PlayerInfos[ControllerId];

		const FVector2D TouchScreen = ConvertTouchToScreen(ControllerId, TouchLocation);

		FVector2D LastLocation = TouchScreen;

		FTouchInfo LastTouchInfo;
		const bool bResult = GetLastTouchInfo(ControllerId, TouchId, LastTouchInfo);
		if (bResult)
		{
			LastLocation = ConvertTouchToScreen(ControllerId, LastTouchInfo.Location);
		}

		FPointerEvent PointerEvent(
			ControllerId,
			TouchId,
			TouchScreen,
			LastLocation,
			1.f,
			true);

		if (!FSlateApplication::Get().ProcessMouseMoveEvent(PointerEvent))
		{
			if (PlayerInfo.PlayerController)
			{
				PlayerInfo.PlayerController->InputTouch(TouchId, ETouchType::Moved, TouchLocation, 1.f, FDateTime::Now(), 0);
			}
		}
		if (TouchId == 0 && PlayerInfos.Num() == 0 && FSlateApplication::Get().IsDragDropping())
		{
			FSlateApplication::Get().SetCursorPos(TouchScreen);
		}
	}
}

void VideoStreaming::OnSlateRendered(SWindow& SlateWindow, void* ViewportRHIPtr)
{
	const auto& Settings = *MyOurActor->StreamSetting;

	float Frame = 1.f / (float)Settings.FrameRate;
	double CurrentTime = FApp::GetCurrentTime();


	UGameViewportClient* GameViewportClient = GEngine->GameViewport;
	if (bIsBroadcasting && GameViewportClient)
	{
		for (auto It = PlayerInfos.CreateConstIterator(); It; ++It)
		{
			int32 ControllerId = It.Key();
			auto PlayerInfo = It.Value();

			if (PlayerInfo.ViewportClient->GetWindow() == SlateWindow.AsShared())
			{
				BroadcastVideoFrame(ControllerId);

				float Diff = CurrentTime - (LastBroadcastTimes.Contains(ControllerId) ? LastBroadcastTimes[ControllerId] : 0.f);
				if (Diff >= Frame)
				{
					LastBroadcastTimes.Add(ControllerId, CurrentTime);

					const FViewportRHIRef* ViewportRHI = (const FViewportRHIRef*)ViewportRHIPtr;
					StartCopyingNextVideoFrame(ControllerId, *ViewportRHI);
				}

			}
		}
	}
	else
	{
		StopStreaming();
	}
}

void VideoStreaming::BroadcastVideoFrame(int32 ControllerId)
{
	auto PlayerReadback = PlayerReadbacks[ControllerId];
	if (PlayerReadback.IsValid())
	{
		if (PlayerReadback->IsBufferReady())
		{
			SendVideoFrameBuffer(ControllerId, (uint8*)PlayerReadback->GetBuffer(), PlayerReadback->GetBufferSize(), PlayerReadback->Resolution.X, PlayerReadback->Resolution.Y);
			PlayerReadback->Unmap();
		}
	}
}


#undef LOCTEXT_NAMESPACE
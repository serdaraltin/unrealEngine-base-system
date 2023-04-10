// Copyright 2020 ByOwls. All Rights Reserved.

#include "MyActor.h"
#include "GenericPlatform/GenericPlatformOutputDevices.h"
#include "JsonObjectConverter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/IPluginManager.h"

#if UE_EDITOR
#include "MovieSceneCaptureDialogModule.h"
#endif

#include "GameFramework/Pawn.h"


#define SHOW_TIME_MESSAGE  0.05f
#define ROUND_PRECISION 1000

AMyActor::AMyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnableStabilizing = true;

	StabilizerStrength = 0.7;

	USceneComponent* DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DummyRoot;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetArrowColor(FLinearColor(0, 0, 255, 0));
	Arrow->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));

	/*Network*/
	AutoIpPort = true;

	TurnOffWindowsDefender = false;

	Ip = "192.168.110.16";

	Port = 7777;

	/*Body*/
	CameraFollow = true;
	AutoFocus = true;

	received = false;

	//EnableMultiCamera = false;

	ShowDebugMessage = false;

	is_MoveUpDown = false;

	FocusSettings.bSmoothFocusChanges = true;
	FocusSettings.FocusSmoothingInterpSpeed = 3.f;

	/*VideoStreaming*/
	EnableStreaming = true;
	StreamSetting = nullptr;

	bAlreadyStreaming = false;

	EnableGUI = true;

	FrameRate = 30;

	Quality = EQuality::Medium;

	/*Operations*/
	MultiplicationByNumber = 1000;

	LeftJoystickMultiplication = 750;
	RightJoystickMultiplication = 750;

	AxisMovement_X = true; AxisMovement_Y = true; AxisMovement_Z = true;

	AxisRotation_Roll = true; AxisRotation_Pitch = true; AxisRotation_Yaw = true;
	Streaming = nullptr;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	TargetLensValue = 35.f;


	InCameraVFX = false;

}


void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	if (TurnOffWindowsDefender) {
		//For clear data receiving we need to turn off windows firewall(run .bat file)
		TurnOffWinFirewall();
	}

	ZeroRotationCoordinate = this->GetActorQuat();
	StartActorRotation = this->GetActorRotation();

	if (StartActorRotation.Yaw >= 0)
		positive = true;
	else
		positive = false;

	CurrentSampleV = this->GetActorLocation();

	CurrentSampleR = this->GetActorRotation();

#if PLATFORM_ANDROID
	StartTCPReceiver("AutoSocketListener", UCameraBlueprintFunctionLibrary::GetLanIp(), FCString::Atof(*GetPortNumber(this)));
#else
	if (AutoIpPort)
	{
		StartTCPReceiver("AutoSocketListener", GetLocalIp4(this), FCString::Atof(*GetPortNumber(this)));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, (FString("PORT: ") += GetPortNumber(this)));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, (FString("IP: ") += GetLocalIp4(this)));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Enter your ip address and port in the application on the phone");
	}
	else
		StartTCPReceiver("SocketListener", Ip, Port);
#endif

	if (CameraFollow)
	{

		UWorld *world = GetWorld();
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

		if (MainCamera == nullptr)
		{
			MainCamera = world->SpawnActor<ACineCameraActor>(this->GetActorLocation(), this->GetActorRotation());
			UE_LOG(LogTemp, Warning, TEXT("Default Camera created!"));

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("User's camera is used. NO default camera creation"));
		}

		if (!ensure(MainCamera != nullptr))
			return;

		CineCamPtr = Cast<ACineCameraActor>(MainCamera);
		if (!CineCamPtr->IsValidLowLevel()) {
			is_focus = false;
		}
		else {
			MainCamera->SetActorLocation(this->GetActorLocation());
			MainCamera->SetActorRotation(this->GetActorRotation());
			CineCamPtrCopy = Cast<ACineCameraActor>(MainCamera);
			is_focus = true;
			CineCamPtr->GetCineCameraComponent()->CurrentAperture = 2.8f;
			CineCamPtr->GetCineCameraComponent()->LensSettings.MaxFocalLength = 1000.f;
			CineCamPtr->GetCineCameraComponent()->LensSettings.MinFocalLength = 4.f;
#if UE_EDITOR
			IMovieSceneCaptureDialogModule& MovieSceneCaptureModule = FModuleManager::Get().LoadModuleChecked<IMovieSceneCaptureDialogModule>("MovieSceneCaptureDialog");

			if (!MovieSceneCaptureModule.GetCurrentCapture().IsValid()) {
				if (EnableGUI)
					LoadMainMenu();
			}
#endif
		}





		//If Using InCamera VFX effect we do not need to set the view from attached camera, we use atached camera just for getting stabilised movements
		if (!InCameraVFX) {
			OurPlayerController->SetViewTargetWithBlend(MainCamera, 0.0f);
		}

		MainCamera->SetActorLocation(this->GetActorLocation());
		MainCamera->SetActorRotation(this->GetActorRotation());
	}



	PhonecordX = 0.f;
	PhonecordY = 0.f;
	PhonecordZ = 0.f;

	DeltacordX = 0.f;
	DeltacordY = 0.f;
	DeltacordZ = 0.f;
	PhoneRotation = FRotator(0, 0, 0);
	DeltaStopTracking = FRotator(0, 0, 0);
	NewActorRotation = this->GetActorRotation();
}


void AMyActor::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxisKey(EKeys::Gamepad_LeftY, this, &AMyActor::OnMoveForward);
	InputComponent->BindAxisKey(EKeys::Gamepad_LeftX, this, &AMyActor::OnMoveRight);
	InputComponent->BindAxisKey(EKeys::Gamepad_RightY, this, &AMyActor::OnMoveUp);

}


void AMyActor::OnMoveForward(const float InValue)
{

	if (InValue == 0) {
		if (RightJoyStickValue == 0) {
			is_UsingJoystick = false;
		}
		ForwardJoyStickValue = 0.f;
	}
	else {
		ForwardJoyStickValue = InValue;
		is_UsingJoystick = true;
		ZeroLocationCoordinate = GetActorLocation();
		DeltacordX = PhonecordX;
		DeltacordY = PhonecordY;
		DeltacordZ = PhonecordZ;
	}

}

void AMyActor::OnMoveRight(const float InValue)
{

	if (InValue == 0) {
		if (ForwardJoyStickValue == 0) {
			is_UsingJoystick = false;
		}
		RightJoyStickValue = 0.f;
	}
	else {
		RightJoyStickValue = InValue;
		is_UsingJoystick = true;
		ZeroLocationCoordinate = GetActorLocation();
		DeltacordX = PhonecordX;
		DeltacordY = PhonecordY;
		DeltacordZ = PhonecordZ;
	}
}

void AMyActor::OnMoveUp(const float InValue)
{

	if (InValue == 0) {
		if (ForwardJoyStickValue == 0 && RightJoyStickValue == 0) {
			is_UsingJoystick = false;
		}
		UpJoyStickValue = 0.f;
	}
	else {
		UpJoyStickValue = -InValue;
		is_UsingJoystick = true;
		ZeroLocationCoordinate = GetActorLocation();
		DeltacordZ = PhonecordZ;
	}

}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CameraFollow) {
		if (!ensure(MainCamera != nullptr))
			return;


		if (is_UsingJoystick) {
			SetActorLocation(GetActorLocation() + ((GetActorForwardVector() * ((LeftJoystickMultiplication / 3) * ForwardJoyStickValue)) * DeltaTime));
			SetActorLocation(GetActorLocation() + ((GetActorRightVector()   * ((LeftJoystickMultiplication / 3) * RightJoyStickValue))* DeltaTime));
			SetActorLocation(GetActorLocation() + ((GetActorUpVector()      * ((RightJoystickMultiplication / 5) * UpJoyStickValue))* DeltaTime));
		}


		if (EnableStabilizing)
		{
			StabilizeFunc(this->GetActorLocation(), this->GetActorRotation());
		}
		else
		{
			MainCamera->SetActorLocation(this->GetActorLocation());
			MainCamera->SetActorRotation(this->GetActorRotation());
		}

		if (AutoFocus && is_focus)
		{
			FocusFunc(DeltaTime);

		}

		if (EnableGUI && MainMenu != nullptr)
			LensStabilizeFunc();
	}

}

void AMyActor::StabilizeFunc(FVector LocationIn, FRotator RotationIn)
{
	if (!ensure(MainCamera != nullptr))
		return;

	FVector LocIn;
	/*LocIn.X = FMath::RoundHalfToEven(LocationIn.X);
	LocIn.Y = FMath::RoundHalfToEven(LocationIn.Y);
	LocIn.Z = FMath::RoundHalfToEven(LocationIn.Z);*/

	LocIn.X = LocationIn.X;
	LocIn.Y = LocationIn.Y;
	LocIn.Z = LocationIn.Z;

	FRotator RotIn;
	/*RotIn.Yaw = FMath::RoundHalfToEven(RotationIn.Yaw);
	RotIn.Pitch = FMath::RoundHalfToEven(RotationIn.Pitch);
	RotIn.Roll = FMath::RoundHalfToEven(RotationIn.Roll);*/

	RotIn.Yaw = RotationIn.Yaw;
	RotIn.Pitch = RotationIn.Pitch;
	RotIn.Roll = RotationIn.Roll;

	if (positive && RotIn.Yaw < -15) {
		float tmpRot = Stabilizer(-180, 0, 180, 360, RotIn.Yaw);
		RotIn.Yaw = tmpRot;
		if (CurrentSampleR.Yaw >= 179.5) {
			CurrentSampleR.Yaw = -180;
			MainCamera->SetActorRotation(CurrentSampleR);
			positive = false;
			return;
		}
	}
	else if (!positive && RotIn.Yaw > 15) {
		float tmpRot = Stabilizer(180, 0, -180, -360, RotIn.Yaw);
		RotIn.Yaw = tmpRot;
		if (CurrentSampleR.Yaw <= -179.5) {
			CurrentSampleR.Yaw = 180;
			MainCamera->SetActorRotation(CurrentSampleR);
			positive = true;
			return;
		}

	}
	else {
		if (GetActorRotation().Yaw >= 0)
			positive = true;
		else
			positive = false;
	}

	float Weight;
	if (InCameraVFX) {
		Weight = Stabilizer(0, 1, 0.5, 0.013, StabilizerStrength);
	}
	else {
		Weight = Stabilizer(0, 1, 0.22, 0.013, StabilizerStrength);
	}


	CurrentSampleV.X = FMath::WeightedMovingAverage(LocIn.X, CurrentSampleV.X, Weight);
	CurrentSampleV.Y = FMath::WeightedMovingAverage(LocIn.Y, CurrentSampleV.Y, Weight);
	CurrentSampleV.Z = FMath::WeightedMovingAverage(LocIn.Z, CurrentSampleV.Z, Weight);

	CurrentSampleR.Yaw = FMath::WeightedMovingAverage(RotIn.Yaw, CurrentSampleR.Yaw, Weight);
	CurrentSampleR.Pitch = FMath::WeightedMovingAverage(RotIn.Pitch, CurrentSampleR.Pitch, Weight);
	CurrentSampleR.Roll = FMath::WeightedMovingAverage(RotIn.Roll, CurrentSampleR.Roll, Weight);

	MainCamera->SetActorLocation(CurrentSampleV);
	MainCamera->SetActorRotation(CurrentSampleR);

}

void AMyActor::FocusFunc(float DeltaTime)
{
	FHitResult HitRes;
	FVector startLoc = CineCamPtr->GetActorLocation();
	FVector endLoc = ((CineCamPtr->GetCineCameraComponent()->GetForwardVector() * 50000) + startLoc);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(HitRes, startLoc, endLoc, ECC_Visibility, CollisionParams)) {
		CineCamPtr->GetCineCameraComponent()->LensSettings.MinimumFocusDistance = 10.f;
		FocusSettings.ManualFocusDistance = HitRes.Distance;
		if (HitRes.Distance <= 600) {
			//we can also use this func in order to lerp interpolation speed by the distance
			FocusSettings.FocusSmoothingInterpSpeed = Stabilizer(0.f, 600.f, 3.2f, 1.7f, HitRes.Distance);
		}
		else {
			if (HitRes.Distance <= 5000) {
				//we can also use this func in order to lerp interpolation speed by the distance
				FocusSettings.FocusSmoothingInterpSpeed = Stabilizer(600.f, 5000.f, 0.6f, 0.05f, HitRes.Distance);
			}
			else {
				//we can also use this func in order to lerp interpolation speed by the distance
				FocusSettings.FocusSmoothingInterpSpeed = Stabilizer(5000.f, 105000.f, 0.05f, 0.0009f, HitRes.Distance);
			}
		}

		CineCamPtr->GetCineCameraComponent()->FocusSettings = FocusSettings;
		//CineCamPtr->GetCineCameraComponent()->FocusSettings.FocusMethod = 
	}

}

float AMyActor::Stabilizer(float OldMin, float OldMax, float NewMin, float NewMax, float OldValue)
{

	float OldRange = (OldMax - OldMin);
	float NewRange = (NewMax - NewMin);
	float NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin;

	return NewValue;
}

void AMyActor::LensStabilizeFunc()
{
	
	float Weight = Stabilizer(0, 1, 0.14, 0.01, MainMenu->SmoothLensValue);

	float CurrentLensValue = FMath::WeightedMovingAverage(TargetLensValue, CineCamPtr->GetCineCameraComponent()->CurrentFocalLength, Weight);

	
	CineCamPtr->GetCineCameraComponent()->CurrentFocalLength = CurrentLensValue;
}

void AMyActor::StartStreaming(const FString& PhoneIP)
{
	int32 resolution = 768;
	int32 quality = 80;
	switch (Quality)
	{
	case EQuality::Low:
		resolution = 512;
		quality = 60;
		break;
	case EQuality::Medium:
		break;
	case EQuality::High:
		resolution = 1000;
		quality = 80;
		break;
	default:
		break;
	}

	StreamSetting = new StreamSettings(resolution, FrameRate, quality, 54232, PhoneIP);
	StreamSetting->IpAddress = PhoneIP;
	Streaming = new VideoStreaming();
	Streaming->StartVideoStream(PhoneIP, this);
	Streaming->StartupModule();
}

void AMyActor::LoadMainMenu()
{
	if (MyBP == nullptr)
		return;


	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	MainMenu = CreateWidget<UMainUserWidget>(OurPlayerController, MyBP);

	if (!ensure(MainMenu != nullptr))
		return;

	MainMenu->SetUp(this);

	MainMenu->SetMyActorPtr(this);

	if (!ensure(MainCamera != nullptr))
		return;

	MainMenu->TargetCamera = MainCamera;


}

void AMyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (&TCPConnectionListenerTimerHandleArr != nullptr)
		GetWorld()->GetTimerManager().ClearTimer(TCPConnectionListenerTimerHandleArr);

	if (ConnectionSocket != NULL)
		ConnectionSocket->Close();
	if (ListenerSocket != NULL)
		ListenerSocket->Close();

	if (EnableStreaming && bAlreadyStreaming) {
		Streaming->ShutdownModule();
		Streaming->StopStreaming();

		delete Streaming;
		delete StreamSetting;
	}

	if (MainMenu != nullptr) {
		MainMenu->Teardown();
	}
}

bool AMyActor::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {
	//CreateTCPConnectionListener
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	//Not created?
	if (!ListenerSocket)
	{
		if (ShowDebugMessage)
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		return false;
	}

	//Start the Listener!
	//thread this eventually
	ZeroLocationCoordinate = GetActorLocation();

	//ZeroRotationCoordinate = GetActorQuat();

	GetWorld()->GetTimerManager().SetTimer(TCPConnectionListenerTimerHandleArr, this, &AMyActor::TCPConnectionListener, 0.008f, true);

	if (ShowDebugMessage)
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, FString::Printf(TEXT("StartTCPReceiver>> Listen socket created")));

	return true;
}

FSocket* AMyActor::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		if (ShowDebugMessage)
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Red, FString::Printf(TEXT("FormatIP4ToNumber >> does not format ip")));
		return NULL;
	}
	else
	{
		if (ShowDebugMessage)
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, FString::Printf(TEXT("FormatIP4ToNumber >> format ip true")));
	}

	//Create Socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	//Set Buffer Size
	int32 NewSize = 0;
	if (ListenSocket == nullptr)
	{
		if (ShowDebugMessage)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("check the IP entered is correct")));
		return nullptr;
	}
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

	//Done!
	return ListenSocket;
}



bool AMyActor::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
{
	//IP Formatting
	FString Temp = TheIP.Replace(TEXT(" "), TEXT(""));

	//String Parts
	TArray<FString> Parts;
	TheIP.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}
	return true;
}

std::string AMyActor::FindSymbolInStr(const std::string & str, std::string symbol)
{
	int posSymbol = str.find(symbol);
	int lengthStr = str.length();
	std::string buffStr = str.substr(posSymbol, lengthStr - posSymbol);

	buffStr = buffStr.substr(buffStr.find(':'), lengthStr - buffStr.find(':'));

	int posComma;
	posComma = buffStr.find(',');
	if (posComma == std::string::npos)
		posComma = buffStr.find('}');
	if (posComma != std::string::npos)
		buffStr.erase(posComma);

	return buffStr.substr(1, buffStr.length());
}

void AMyActor::TCPConnectionListener()
{
	if (!ListenerSocket) return;

	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;


	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
	{
		//Already have a Connection? destroy previous
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}

		//New Connection receive!
		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("TCP Received Socket Connection"));

		if (ShowDebugMessage)
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, FString::Printf(TEXT("TCPConnectionListener >> TCP Received Socket Connection")));

		FTimerHandle handle;

		if (ConnectionSocket != NULL) {
			TCPSocketListener();
			//GetWorldTimerManager().SetTimer(handle, this, &AMyActor::TCPSocketListener, 0.01f, true);
		}

		if (!is_MoveUpDown) {
			
			GetWorldTimerManager().SetTimer(handle, this, &AMyActor::OnMoveUpDown, 4.f);
			is_MoveUpDown = true;
		}

		if (EnableStreaming) {
			if (!bAlreadyStreaming) {
				TSharedRef<FInternetAddr> Adr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
				ConnectionSocket->GetPeerAddress(*Adr);
				uint32 YourInt;
				Adr->GetIp(YourInt);
				FString IntAsString = FString::FromInt(YourInt);
				StartStreaming(Adr->ToString(false));
				bAlreadyStreaming = true;
			}
		}
	}
}

FString AMyActor::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	return FString(cstr.c_str());
}

void AMyActor::DisableAutoFocus()
{

	AutoFocus = false;
	FocusSettings.ManualFocusDistance = 100000.f;
	//FocusSettings.FocusMethod = ECameraFocusMethod::None;
	CineCamPtr->GetCineCameraComponent()->CurrentAperture = 2.8f;
	CineCamPtr->GetCineCameraComponent()->FocusSettings = FocusSettings;
}

void AMyActor::EnableAutoFocus()
{
	AutoFocus = true;
	// FocusSettings.FocusMethod = ECameraFocusMethod::Manual;
}

void AMyActor::TCPSocketListener()
{
	if (!ConnectionSocket) return;

	TArray<uint8> ReceivedData;

	static int32 delsize = 0;

	uint32 Size;

	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(FMath::Min(Size, 65507u), Size);

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
	}

	if (ReceivedData.Num() <= 0) {
		return;
	}

	//String From Binary Array
	FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	FReceiveStruct rec;

	if (FJsonObjectConverter::JsonObjectStringToUStruct(ReceivedUE4String, &rec, 0, 0)) {
		if (rec.codeRequest == 111) {

			FString path = FPaths::EngineDir();
			path.Append("Plugins/Marketplace/VirtualAppPlugin/Content");

			bool quit = false;
			FSendStruct sendStruct;

#if UE_EDITOR
			if (FPaths::DirectoryExists(path)) {
				sendStruct.isEligible = 10;
			}
			else {
				sendStruct.isEligible = 20;
				quit = true;
			}
#else
#if (UE_BUILD_SHIPPING || UE_BUILD_TEST || UE_BUILD_DEVELOPMENT  )
			sendStruct.isEligible = 10;
#endif
#endif

			sendStruct.projectName = FApp::GetProjectName();

			sendStruct.engineVersion = UKismetSystemLibrary::GetEngineVersion().Left(4);

			TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("Camera");

			if (plugin.IsValid()) {
				sendStruct.pluginVersion = plugin->GetDescriptor().VersionName;
			}
			FModuleManager& Module = FModuleManager::Get();


			if (Module.IsModuleLoaded("DisplayCluster"))
			{
				sendStruct.enabledPlugins.Add("nDisplay");
			}
			if (Module.IsModuleLoaded("GoogleVRHMD"))
			{
				sendStruct.enabledPlugins.Add("GoogleVR");
			}
			if (Module.IsModuleLoaded("GoogleARCoreBase"))
			{
				sendStruct.enabledPlugins.Add("GoogleAR");
			}
			if (Module.IsModuleLoaded("VirtualCamera"))
			{
				sendStruct.enabledPlugins.Add("VirtualCameraEpic");
			}
			if (Module.IsModuleLoaded("PixelStreaming"))
			{
				sendStruct.enabledPlugins.Add("PixelStreaming");
			}


			FString StrToSend;
			FJsonObjectConverter::UStructToJsonObjectString(sendStruct, StrToSend);


			int32 SendReader;

			TCHAR *serializedChar = StrToSend.GetCharArray().GetData();
			int32 size = FCString::Strlen(serializedChar);

			ConnectionSocket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, SendReader);

			/*if (quit == true)
				OnMoveUpDown();*/

			received = true;
			return;
		}
	}


	const std::string cstr(reinterpret_cast<const char*>(ReceivedData.GetData()), ReceivedData.Num());

	FVector LocationStringData = GetActorLocation();
	FRotator RotationStringData = GetActorRotation();
	FQuat RotationQuat = RotationStringData.Quaternion();
	float cordX;
	float cordY;
	float cordZ;

	float multiplicatedX = PhonecordX;
	float multiplicatedY = PhonecordY;
	float multiplicatedZ = PhonecordZ;

	PhonecordX = ((((FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))) * 100) / (ROUND_PRECISION))))) * (-1);
	PhonecordY = (((FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "x").c_str()))) * 100) / (ROUND_PRECISION))));
	PhonecordZ = (((FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "y").c_str()))) * 100) / (ROUND_PRECISION))));

	multiplicatedX = (PhonecordX - multiplicatedX);
	multiplicatedY = (PhonecordY - multiplicatedY);
	multiplicatedZ = (PhonecordZ - multiplicatedZ);

	if (StartActorRotation.Yaw >= 135 || StartActorRotation.Yaw <= -135) {

		cordX = ZeroLocationCoordinate.X - multiplicatedX * MultiplicationByNumber;
		cordY = ZeroLocationCoordinate.Y - multiplicatedY * MultiplicationByNumber;
		cordZ = ZeroLocationCoordinate.Z + multiplicatedZ * MultiplicationByNumber;

	}
	else if (StartActorRotation.Yaw < 135 && StartActorRotation.Yaw >= 45) {
		cordY = ZeroLocationCoordinate.Y + multiplicatedX * MultiplicationByNumber;
		cordX = ZeroLocationCoordinate.X - multiplicatedY * MultiplicationByNumber;
		cordZ = ZeroLocationCoordinate.Z + multiplicatedZ * MultiplicationByNumber;
	}
	else if (StartActorRotation.Yaw > -135 && StartActorRotation.Yaw <= -45) {
		cordY = ZeroLocationCoordinate.Y - multiplicatedX * MultiplicationByNumber;
		cordX = ZeroLocationCoordinate.X + multiplicatedY * MultiplicationByNumber;
		cordZ = ZeroLocationCoordinate.Z + multiplicatedZ * MultiplicationByNumber;
	}
	else {
		cordX = ZeroLocationCoordinate.X + multiplicatedX * MultiplicationByNumber;
		cordY = ZeroLocationCoordinate.Y + multiplicatedY * MultiplicationByNumber;
		cordZ = ZeroLocationCoordinate.Z + multiplicatedZ * MultiplicationByNumber;
	}

	ZeroLocationCoordinate.X = cordX;
	ZeroLocationCoordinate.Y = cordY;
	ZeroLocationCoordinate.Z = cordZ;

	LocationStringData = FVector(AxisMovement_X ? cordX : GetActorLocation().X, AxisMovement_Y ? cordY : GetActorLocation().Y, AxisMovement_Z ? cordZ : GetActorLocation().Z);

	if (!is_UsingJoystick) {
		SetActorLocation(LocationStringData);
	}

	RotationQuat = FQuat(
		AxisRotation_Roll ? (FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "r").c_str()))) * ROUND_PRECISION) / ROUND_PRECISION) : 0,
		AxisRotation_Pitch ? (FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "p").c_str()))) * ROUND_PRECISION) / ROUND_PRECISION) * (-1) : 0,
		AxisRotation_Yaw ? (FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "yaw").c_str()))) * ROUND_PRECISION) / ROUND_PRECISION) * (-1) : 0,
		(FCString::Atof(*(FString(FindSymbolInStr(cstr, "ww").c_str())))));

	PhoneRotation = FRotator(FQuat(
		(FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "r").c_str()))) * ROUND_PRECISION) / ROUND_PRECISION),
		(FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "p").c_str()))) * ROUND_PRECISION) / ROUND_PRECISION) * (-1),
		(FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "yaw").c_str()))) * ROUND_PRECISION) / ROUND_PRECISION) * (-1),
		(FCString::Atof(*(FString(FindSymbolInStr(cstr, "ww").c_str()))))));

	//UE_LOG(LogTemp, Warning, TEXT("1 %f %f %f"), PhoneRotation.Roll, PhoneRotation.Pitch, PhoneRotation.Yaw);
	//UE_LOG(LogTemp, Warning, TEXT("2 %f %f %f"), DeltaStopTracking.Roll, DeltaStopTracking.Pitch, DeltaStopTracking.Yaw);

	SetActorRotation((FRotator(RotationQuat) - DeltaStopTracking) + NewActorRotation);

	if (ShowDebugMessage)
	{
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Cyan, "W:= " + FString::SanitizeFloat(RotationQuat.W));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Blue, "Yaw:= " + FString::SanitizeFloat(RotationQuat.Z));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, "Pitch:= " + FString::SanitizeFloat(RotationQuat.Y));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Red, "Roll:= " + FString::SanitizeFloat(RotationQuat.X));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Emerald, "////////////////////////rotation");
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Emerald, "Z:= " + FString::SanitizeFloat(LocationStringData.Z));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::White, "Y:= " + FString::SanitizeFloat(LocationStringData.Y));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Yellow, "X:= " + FString::SanitizeFloat(LocationStringData.X));
	}
}

//UFUNCTION
FVector AMyActor::GetPhoneLocation()
{
	if (!ConnectionSocket) return FVector(0.0f, 0.0f, 0.0f);

	TArray<uint8> ReceivedData;

	static int32 delsize = 0;

	uint32 Size;

	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(FMath::Min(Size, 65507u), Size);

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
	}

	if (ReceivedData.Num() <= 0) return FVector(0.0f, 0.0f, 0.0f);

	//String From Binary Array
	FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	const std::string cstr(reinterpret_cast<const char*>(ReceivedData.GetData()), ReceivedData.Num());

	return FVector(FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))), FCString::Atof(*(FString(FindSymbolInStr(cstr, "x").c_str()))), FCString::Atof(*(FString(FindSymbolInStr(cstr, "y").c_str()))));
}

FQuat AMyActor::GetPhoneRotation()
{
	if (!ConnectionSocket) return FQuat();

	TArray<uint8> ReceivedData;

	static int32 delsize = 0;

	uint32 Size;

	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(FMath::Min(Size, 65507u), Size);

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
	}

	if (ReceivedData.Num() <= 0) return FQuat();

	//String From Binary Array
	FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	const std::string cstr(reinterpret_cast<const char*>(ReceivedData.GetData()), ReceivedData.Num());

	return FQuat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "r").c_str()))), FCString::Atof(*(FString(FindSymbolInStr(cstr, "p").c_str()))), FCString::Atof(*(FString(FindSymbolInStr(cstr, "yaw").c_str()))), FCString::Atof(*(FString(FindSymbolInStr(cstr, "ww").c_str()))));
}

const FString AMyActor::GetLocalIp4(UObject * WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld * World = WorldContextObject->GetWorld())
		{
			bool canBind = false;
			TSharedRef <FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
			return (localIp->IsValid() ? localIp->ToString(false) : World->URL.Host);
		}
	}
	return "WorldContextObject is FALSE";
}


void AMyActor::TurnOffWinFirewall()
{

	FOutputDevice* dev = FGenericPlatformOutputDevices::GetLog();
	FString path = FPaths::EngineDir().Append("Plugins/Marketplace/VirtualAppPlugin/Content/WinFirewall/CheckForFirewall.exe");
	bool exist = FPaths::FileExists(path);


	if (exist)
	{
		FProcHandle handle = FPlatformProcess::CreateProc(*path, nullptr, false, true, true, nullptr, 0, nullptr, nullptr);
	}

}

void AMyActor::OnMoveUpDown()
{
	/*if (received)
		return;*/
#if UE_EDITOR
	FString path = FPaths::EngineDir();
	path.Append("Plugins/Marketplace/VirtualAppPlugin/Content");
	if (FPaths::DirectoryExists(path)) {

	}
	else {

		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		OurPlayerController->ConsoleCommand("quit");

		UE_LOG(LogTemp, Warning, TEXT("Unauthorized using plugin"));
	}
#endif
}

const FString AMyActor::GetPortNumber(UObject * WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld * World = WorldContextObject->GetWorld())
		{
			return FString::FromInt(World->URL.Port);
		}
	}
	return "WorldContextObject is FALSE";
}

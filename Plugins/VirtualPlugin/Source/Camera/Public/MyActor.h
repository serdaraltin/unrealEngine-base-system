// Copyright 2020 ByOwls. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Networking.h"
#include "Engine.h"
#include <string>
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CineCameraComponent.h"
#include "CineCameraActor.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "InObjectCamera.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "StreamSettings.h"
#include "VideoStreaming.h"
#include "MainUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"

#include "MyActor.generated.h"

UENUM(BlueprintType)
enum class EQuality : uint8
{
	Low UMETA(DisplayName = "Low quality"),
	Medium UMETA(DisplayName = "Medium quality"),
	High UMETA(DisplayName = "High quality")
};


USTRUCT()
struct FSendStruct
{
	GENERATED_BODY()
public:
	UPROPERTY()
		int32 isEligible;

	UPROPERTY()
		FString projectName;

	UPROPERTY()
		FString engineVersion;

	UPROPERTY()
		FString pluginVersion;

	UPROPERTY()
		TArray<FString> enabledPlugins;

};

USTRUCT()
struct FReceiveStruct
{
	GENERATED_BODY()
public:
	UPROPERTY()
		int32 codeRequest;
};

UCLASS()
class AMyActor : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Stabilization", meta = (EditCondition = "CameraFollow"))
		bool EnableStabilizing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Stabilization", meta = (UIMin = 0, UIMax = 1.0, EditCondition = "EnableStabilizing"))
		float StabilizerStrength;

	//Choose your Level Sequence from the scene to run it automaticly when recording started
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Sequencer")
		ALevelSequenceActor* LevelSequenceActor;


	/*if true -> the camera will follow the objcet*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Body")
		bool CameraFollow;

	/*if true -> shows messages with a frequency of 0.05*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Body")
		bool ShowDebugMessage;

	//Start Camera in the scene
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Body", meta = (EditCondition = "CameraFollow"))
		ACameraActor* MainCamera;

	///*if true -> you can use multiple CineCams*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Body", meta = (EditCondition = "CameraFollow"))
	//	bool EnableMultiCamera;

	//An array of Cameras that you can switch in User Interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Body", meta = (EditCondition = "CameraFollow"))
		TArray<ACineCameraActor*> MultiCineCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Body", meta = (EditCondition = "CameraFollow"))
		bool AutoFocus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | User Interface", meta = (EditCondition = "CameraFollow"))
		bool EnableGUI;

	/*if true -> video streaming will launch from the editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Video Streaming")
		bool EnableStreaming;

	/** Stream frame rate. Do not change to high unless you are sure your mobile device can handle it*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Video Streaming", meta = (UIMin = 5, ClampMin = 1, UIMax = 75, EditCondition = "EnableStreaming"))
		int32 FrameRate;

	/*Stream quality. Do not change to high unless you are sure your mobile device can handle it*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Video Streaming", meta = (EditCondition = "EnableStreaming"))
		EQuality Quality;

	/*if you want to automatically detect ip and port for your object*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Network")
		bool AutoIpPort;

	/*ip address local network*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Network", meta = (EditCondition = "! AutoIpPort"))
		FString Ip;

	/*port local network*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Network", meta = (EditCondition = "! AutoIpPort"))
		int32 Port;

	/*if you do not want to automatically turn off Windows Firewall uncheck this box*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Network")
		bool TurnOffWindowsDefender;

	/*the ratio of the coordinates of the phone to the coordinates of the object in the scene*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Operations | Variable", meta = (EditCondition = false))
		int32 MultiplicationByNumber;

	/* transfer X coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Operations | Сoordinates | Movement")
		bool AxisMovement_X;

	/* transfer Y coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Operations | Сoordinates | Movement")
		bool AxisMovement_Y;

	/* transfer Z coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Operations | Сoordinates | Movement")
		bool AxisMovement_Z;

	/* transfer Roll rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Operations | Сoordinates | Rotation")
		bool AxisRotation_Roll;

	/* transfer Pitch rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Operations | Сoordinates | Rotation")
		bool AxisRotation_Pitch;

	/* transfer Yaw rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | Operations | Сoordinates | Rotation")
		bool AxisRotation_Yaw;

	/*If Using InCamera VFX effect we do not need to set the view from attached camera, we use atached camera just for getting stabilised movements*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPhone | InCamera", meta = (EditCondition = "CameraFollow"))
		bool InCameraVFX;

public:

	/*if you need to get phone coordinates for your functions*/
	UFUNCTION(BlueprintPure, Category = "ARPhone")
		FVector GetPhoneLocation();

	/*if you need to get phone rotation for your functions*/
	UFUNCTION(BlueprintPure, Category = "ARPhone")
		FQuat GetPhoneRotation();

	/*if you need to get the local address of the computer*/
	UFUNCTION(BlueprintPure, Category = "ARPhone")
		static const FString GetLocalIp4(UObject* WorldContextObject);

	/*if you need to get the port of the computer*/
	UFUNCTION(BlueprintPure, Category = "ARPhone")
		static const FString GetPortNumber(UObject* WorldContextObject);

	/*create a string from a byte array*/
	UFUNCTION(BlueprintCallable, Category = "ARPhone")
		FString StringFromBinaryArray(TArray<uint8> BinaryArray);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BPClasses")
		TSubclassOf<UUserWidget> MyBP;

	class StreamSettings* StreamSetting;
	class VideoStreaming* Streaming;


	void DisableAutoFocus();
	void EnableAutoFocus();

	float Stabilizer(float OldMin, float OldMax, float NewMin, float NewMax, float OldValue);

	void LensStabilizeFunc();

	float LeftJoystickMultiplication;
	float RightJoystickMultiplication;

	float TargetLensValue;

	ACineCameraActor* CineCamPtr;
	ACineCameraActor* CineCamPtrCopy;
	FRotator StartActorRotation;

	FRotator NewActorRotation;

	/*start coordinate actor in world*/
	FVector ZeroLocationCoordinate;

	/*start coordinate actor in world*/
	FQuat ZeroRotationCoordinate;

	FRotator DeltaStopTracking;

	FRotator PhoneRotation;

	float PhonecordX;
	float PhonecordY;
	float PhonecordZ;

	FVector CurrentSampleV;
	FRotator CurrentSampleR;

	//Whether Actor rotation .Yaw positive or negative
	bool positive;


	void TurnOffWinFirewall();

private:

	UArrowComponent* Arrow;

	class UMainUserWidget* MainMenu;

	TSubclassOf<UUserWidget> MenuClass;

	bool is_focus;

	bool bAlreadyStreaming;

	float ForwardJoyStickValue;
	float RightJoyStickValue;
	float UpJoyStickValue;
	bool is_UsingJoystick;

	float DeltacordX;
	float DeltacordY;
	float DeltacordZ;

	FCameraFocusSettings FocusSettings;

	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;

	/*timer variable*/
	FTimerHandle TCPConnectionListenerTimerHandleArr;

	/*constructor*/
	AMyActor();

	/*start tcp receiver*/
	bool StartTCPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	/*create socket*/
	FSocket* CreateTCPConnectionListener(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
	);

	void StabilizeFunc(FVector LocationIn, FRotator RotationIn);

	void FocusFunc(float DeltaTime);


	void StartStreaming(const FString& PhoneIP);

	void LoadMainMenu();

	/*func listner on SetTimer()*/
	void TCPConnectionListener();

	/*the main function where all the calculations take place*/
	void TCPSocketListener();

	/*ip formatting*/
	bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);

	/*develop a function on types out of the box FString*/
	std::string FindSymbolInStr(const std::string &str, std::string symbol);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnMoveForward(const float InValue);

	void OnMoveRight(const float InValue);

	void OnMoveUp(const float InValue);

	void OnMoveUpDown();

	bool is_MoveUpDown;
	bool received;



protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;

};

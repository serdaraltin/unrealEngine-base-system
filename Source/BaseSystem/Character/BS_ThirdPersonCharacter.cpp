// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_ThirdPersonCharacter.h"
#include "../Function_Library/BS_Library.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABS_ThirdPersonCharacter::ABS_ThirdPersonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);
	// Instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	// Crate a Camera boom
	CameraBoomComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom Companent"));
	// Attach the Camera Boom to Root Component
	CameraBoomComp->SetupAttachment(this->RootComponent);
	CameraBoomComp->TargetArmLength = 450.f;
	CameraBoomComp->bUsePawnControlRotation = true;
	// Create a Camera 
	FollowCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Companent"));
	// Attach the Camera to end of boom
	FollowCameraComp->SetupAttachment(CameraBoomComp, USpringArmComponent::SocketName);
	// Camera does not rotate to arm
	FollowCameraComp->bUsePawnControlRotation = false; 
	
}

void ABS_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABS_ThirdPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//Function Library Line Trace function test
	//FHitResult FHitResult = ABS_Library::MakeSingleLineTrace(this);

	//delay test
	//GEngine->AddOnScreenDebugMessage(-1,-1, FColor::Cyan,FString::Printf(TEXT("ms : %f"),DeltaSeconds));
}

// Called to bind functionality to input
void ABS_ThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Move Forward / Backward",this, &ABS_ThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left",this,&ABS_ThirdPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void ABS_ThirdPersonCharacter::MoveForward(float Value)
{
	if((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}


void ABS_ThirdPersonCharacter::MoveRight(float Value)
{
	if((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BS_ThirdPersonCharacter.generated.h"


UCLASS(Config=Game)
class BASE_SYSTEM_API ABS_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *CameraBoomComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *FollowCameraComp;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	
public:
	// Sets default values for this character's properties
	ABS_ThirdPersonCharacter();
	// Called every frame
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class USpringArmComponent *GetCameraBoomComp() const {return CameraBoomComp;}
	FORCEINLINE class UCameraComponent *GetFollowCameraComp() const {return  FollowCameraComp;}
};

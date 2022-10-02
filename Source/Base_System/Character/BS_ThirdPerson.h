// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BS_ThirdPerson.generated.h"


UCLASS(config=Game)
class ABS_ThirdPerson : public ACharacter
{
	GENERATED_BODY()

	// Camera Boom positioning the camera behind the character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *CameraBoomComponent;

	// Fallow Camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *FollowCameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called per frame 
	virtual void Tick(float DeltaSeconds) override;

	// Called for forwards/backward input 
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);

	// Called via input to turn at a given rate.
	void TurnAtRate(float Rate);

	// Called via input to turn look up/down at a given rate.
	void LookUPAtRate(float Rate);
	
public:
	// Sets default values for this character's properties
	ABS_ThirdPerson();
	
	// Called every frame
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns Camera Boom subobject
	FORCEINLINE class USpringArmComponent *GetCameraBoomCompanent() const { return  CameraBoomComponent; }
	// Return Fallow Camera subobject
	FORCEINLINE class UCameraComponent *GetFallowCameraCompanent() const { return  FollowCameraComponent; }
	
};

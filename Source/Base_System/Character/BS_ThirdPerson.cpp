// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_ThirdPerson.h"


// Sets default values
ABS_ThirdPerson::ABS_ThirdPerson()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABS_ThirdPerson::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABS_ThirdPerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABS_ThirdPerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


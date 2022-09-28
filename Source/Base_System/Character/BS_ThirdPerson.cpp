// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_ThirdPerson.h"


// Sets default values
ABS_ThirdPerson::ABS_ThirdPerson()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("bbbbbb"));
	this->StaticMesh->SetupAttachment(GetMesh());

}

void ABS_ThirdPerson::BeginPlay()
{
	Super::BeginPlay();
}


void ABS_ThirdPerson::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("sasadadas"),true);
}


// Called to bind functionality to input
void ABS_ThirdPerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


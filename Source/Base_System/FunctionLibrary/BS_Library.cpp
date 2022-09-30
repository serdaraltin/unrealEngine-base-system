// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_Library.h"


// Sets default values
ABS_Library::ABS_Library()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABS_Library::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABS_Library::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


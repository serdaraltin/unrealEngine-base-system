// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BS_Library.generated.h"

UCLASS(Config=Game)
class BASE_SYSTEM_API ABS_Library : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABS_Library();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category= "Collision")
	static UPARAM(DisplayName = "Hit Result") FHitResult MakeSingleLineTrace(
		AActor* AIgnoreActor, float FDistance = 1000.f,
	    bool bDrawDebugLine = true, FLinearColor DebugColor = FLinearColor::Blue,
	    bool bPrintHitActorName = true, FLinearColor NameColor = FLinearColor::Blue, bool bRandomColor = false);
};

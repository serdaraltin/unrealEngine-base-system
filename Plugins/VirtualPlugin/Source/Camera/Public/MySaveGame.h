// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 *
 */
UCLASS()
class CAMERA_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()


public:

	UMySaveGame(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Interface)
		float FocusDistance;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		bool Is_DebugPlaneEnabled;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		bool Is_AutoFocusEnabled;

	UPROPERTY(VisibleAnywhere, Category = Interface)
		float ApertureValue;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		float LensValue;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		float LensValueMilimiters;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		float SmoothLensValue;
	UPROPERTY(EditAnywhere, Category = Interface)
		int32 FilmbackPresetIndex;

	UPROPERTY(VisibleAnywhere, Category = Interface)
		float StabilizationValue;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		bool Is_StabilizationEnabled;

	UPROPERTY(VisibleAnywhere, Category = Interface)
		uint32 PhoneMultiplicationValue;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		float LeftMultiplicationValue;
	UPROPERTY(VisibleAnywhere, Category = Interface)
		float RightMultiplicationValue;

	UPROPERTY(VisibleAnywhere, Category = Interface)
		uint32 ActiveWidgetIndex;

};

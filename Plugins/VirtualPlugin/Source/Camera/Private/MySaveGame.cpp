// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

UMySaveGame::UMySaveGame(const FObjectInitializer& ObjectInitializer)
{

	SaveSlotName = "SavedInterfaceSettings";
	UserIndex = 1;

	FocusDistance = 3.f;
	Is_DebugPlaneEnabled = false;
	ApertureValue = 3.f;
	LensValue = 7.f;
	LensValueMilimiters = 35.f;
	SmoothLensValue = 0.6f;
	FilmbackPresetIndex = 2;
	Is_AutoFocusEnabled = true;

	StabilizationValue = 0.7;
	Is_StabilizationEnabled = true;
	PhoneMultiplicationValue = 1000;
	LeftMultiplicationValue = 750;
	RightMultiplicationValue = 750;
	ActiveWidgetIndex = 0;

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_ThirdPersonGameMode.h"
#include "BS_ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABS_ThirdPersonGameMode::ABS_ThirdPersonGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Base_System/ThirdPerson/Blueprints/BP_BS_ThirdPersonCharacter"));

	if(PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

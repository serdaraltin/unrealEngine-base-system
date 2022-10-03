// Copyright Epic Games, Inc. All Rights Reserved.


#include "BS_ThirdPersonGameMode.h"
#include "BS_ThirdPerson.h"
#include "UObject/ConstructorHelpers.h"

ABS_ThirdPersonGameMode::ABS_ThirdPersonGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Base_System/ThirdPerson/Blueprints/BP_TP_Adam"));

	if(PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

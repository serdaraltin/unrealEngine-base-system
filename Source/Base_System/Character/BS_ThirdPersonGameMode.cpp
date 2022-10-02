// Copyright Epic Games, Inc. All Rights Reserved.

#include "BS_ThirdPersonGameMode.h"
#include "BS_ThirdPerson.h"
#include "UObject/ConstructorHelpers.h"

ABS_ThirdPersonGameMode::ABS_ThirdPersonGameMode()
{
	// Set default pawn class to our Bluprint character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawBPClass(TEXT("/Game/BS_ThirdPerson/Blueprints/BS_ThirdPersonCharacter"));

	if(PlayerPawBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawBPClass.Class;
	}
}

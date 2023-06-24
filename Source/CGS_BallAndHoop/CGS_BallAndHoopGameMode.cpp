// Copyright Epic Games, Inc. All Rights Reserved.

#include "CGS_BallAndHoopGameMode.h"
#include "CGS_BallAndHoopCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACGS_BallAndHoopGameMode::ACGS_BallAndHoopGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

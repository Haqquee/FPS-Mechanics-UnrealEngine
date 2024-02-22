// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterMechanicsGameMode.h"
#include "ShooterMechanicsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShooterMechanicsGameMode::AShooterMechanicsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

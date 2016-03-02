// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Final_Project.h"
#include "Final_ProjectGameMode.h"
#include "Final_ProjectCharacter.h"

AFinal_ProjectGameMode::AFinal_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

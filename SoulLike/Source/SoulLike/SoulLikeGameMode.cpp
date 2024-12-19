// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulLikeGameMode.h"
#include "SoulLikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoulLikeGameMode::ASoulLikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpecialSoul/Public/Game/SpecialSoulGameMode.h"

#include "Player/CBasePlayer.h"
#include "Player/CPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASpecialSoulGameMode::ASpecialSoulGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ACBasePlayer> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/BP_BasePlayer.BP_BasePlayer_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<ACPlayerController> PlayerControllerBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/BP_PlayerController.BP_PlayerController_C'"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
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
	static ConstructorHelpers::FClassFinder<ACBasePlayer> PlayerPawnBPClass(
		TEXT("/Script/Engine.Blueprint'/Game/Player/BP_BasePlayer.BP_BasePlayer_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<ACPlayerController> PlayerControllerBPClass(
		TEXT("/Script/Engine.Blueprint'/Game/Player/BP_PlayerController.BP_PlayerController_C'"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ASpecialSoulGameMode::PrintRegenDataMap()
{
	// for (const auto& Pair : RegenDataMap)
	// {
	// 	UE_LOG(LogTemp, Log,
	// 	       TEXT(
	// 		       "RegenDataMap || ID: %d) MeleeEnemyCount: %d, RangeEnemyCount: %d, MidleBossCount: %d, FinalBossCount: %d"
	// 	       ),
	// 	       Pair.Key, Pair.Value.MeleeEnemyCount, Pair.Value.RangeEnemyCount, Pair.Value.MidleBossCount,
	// 	       Pair.Value.FinalBossCount);
	// }

	// 초기 데이터 세팅
	if (RegenDataMap.Num() > 0)
		UpdateRegenInfo(1);
}


void ASpecialSoulGameMode::UpdateRegenInfo(const int32 Level)
{
	const auto& StatData = RegenDataMap[Level];
	RegenInfo.ID = StatData.ID;
	RegenInfo.MeleeEnemyCount = StatData.MeleeEnemyCount;
	RegenInfo.RangeEnemyCount = StatData.RangeEnemyCount;
	RegenInfo.MidleBossCount = StatData.MidleBossCount;
	RegenInfo.FinalBossCount = StatData.FinalBossCount;
}

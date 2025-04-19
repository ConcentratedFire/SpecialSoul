// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpecialSoul/Public/Game/SpecialSoulGameMode.h"

#include "EngineUtils.h"
#include "Game/CGameState.h"
#include "Player/CBasePlayer.h"
#include "Player/CPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Utility/CDataSheetUtility.h"

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

void ASpecialSoulGameMode::BeginPlay()
{
	Super::BeginPlay();
	DataSheetUtility = NewObject<UCDataSheetUtility>(this);
}

void ASpecialSoulGameMode::ReadExcelData()
{
	DataSheetUtility->FetchGoogleSheetData<FYasuoAttackData>("Yasuo", "A1", "H8", YasuoAttackDataMap);
	DataSheetUtility->FetchGoogleSheetData<FYasuoMoveData>("YasuoMove", "A1", "D5", YasuoMoveDataMap);
	DataSheetUtility->FetchGoogleSheetData<FJinxAttackData>("Jinx", "A1", "G8", JinxAttackDataMap);
	DataSheetUtility->FetchGoogleSheetData<FUpgradeData>("Upgrade", "A1", "G6", UpgradeDataMap);
	DataSheetUtility->FetchGoogleSheetData<FRegenData>("Regen", "A1", "E23", RegenDataMap);

	DataSheetUtility->OnDataFetched.AddDynamic(this, &ASpecialSoulGameMode::SetData);
}

void ASpecialSoulGameMode::SetData()
{
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
	{
		Cast<ACPlayerState>((*It)->GetPlayerState())->SRPC_SetInitialData();
	}

	// 초기 데이터 세팅
	if (RegenDataMap.Num() > 0)
		UpdateRegenInfo(1);
}


void ASpecialSoulGameMode::UpdateRegenInfo(const int32 Level)
{
	if (!RegenDataMap.Contains(Level)) return;
	const auto& StatData = RegenDataMap[Level];
	RegenInfo.ID = StatData.ID;
	RegenInfo.MeleeEnemyCount = StatData.MeleeEnemyCount;
	RegenInfo.RangeEnemyCount = StatData.RangeEnemyCount;
	RegenInfo.MidleBossCount = StatData.MidleBossCount;
	RegenInfo.FinalBossCount = StatData.FinalBossCount;

	auto GS = GetGameState<ACGameState>();
	GS->CalcEnemyRegenTime(RegenInfo);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		bIsStartRegen = true;
	}, 3.0f, false);
}

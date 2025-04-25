// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpecialSoul/Public/Game/SpecialSoulGameMode.h"

#include "EngineUtils.h"
#include "Game/CGameState.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpectatorPawn.h"
#include "Player/CBasePlayer.h"
#include "Player/CPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Utility/CDataSheetUtility.h"

ASpecialSoulGameMode::ASpecialSoulGameMode()
{
	// use our custom PlayerController class
	// PlayerControllerClass = ACPlayerController::StaticClass();

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

	bStartPlayersAsSpectators = true;
	SpectatorClass = ASpectatorPawn::StaticClass();
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

void ASpecialSoulGameMode::SpawnPlayerCharacter()
{
	APlayerStart* Start = nullptr;
	// 월드 내 PlayerStart 검색
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		Start = *It;
		break;
	}

	if (!Start)
	{
		UE_LOG(LogTemp, Error, TEXT("[Spawn] No PlayerStart found"));
		return;
	}

	auto GS=GetGameState<ACGameState>();
	for (auto PlayerState : GS->PlayerArray)
	{
		if (PlayerState && PlayerState->GetOwner())
		{
			if (ACPlayerController* PC = Cast<ACPlayerController>(PlayerState->GetOwner()))
			{
				// 역할에 따라 스폰할 캐릭터 클래스 선택	
				TSubclassOf<APawn> ClassToSpawn = (PC->bPlayYasuo) ? YasuoClass : JinxClass;
				if (!ClassToSpawn) continue;

				FTransform SpawnTransform(Start->GetActorRotation(), Start->GetActorLocation());
				APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(ClassToSpawn, SpawnTransform);

				if (!IsValid(SpawnedPawn))
				{
					UE_LOG(LogTemp, Error, TEXT("[Spawn] Failed to spawn pawn of class %s"), *ClassToSpawn->GetName());
					return;
				}
				// 컨트롤러가 스폰된 Pawn 소유
				PC->Possess(SpawnedPawn);
				UE_LOG(LogTemp, Warning, TEXT("[Spawn] %s possessed %s"), *PC->GetName(), *SpawnedPawn->GetName());
			}
		}
	}
}

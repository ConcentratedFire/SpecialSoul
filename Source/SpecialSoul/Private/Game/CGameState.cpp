// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameState.h"

#include "EngineUtils.h"
#include "SpecialSoul.h"
#include "Game/SpecialSoulGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"
#include "UI/HUD/GameHUD.h"
#include "Utility/CDataSheetUtility.h"

ACGameState::ACGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACGameState::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// 서버일때만 데이터 읽어오기
	if (HasAuthority())
	{
		GM = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode());
		if (GM->DataSheetUtility)
		{
			GM->ReadExcelData();
			ReadExcelData(GM->DataSheetUtility);
		}

		for (TActorIterator<ACObjectPoolManager> It(GetWorld(), ACObjectPoolManager::StaticClass()); It; ++It)
		{
			ObjectPoolManager = *It;
			ObjectPoolManager->InitSettings();
		}
	}
}

void ACGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority()) return;

	if (GM && GM->bIsStartRegen)
	{
		if (ObjectPoolManager)
		{
			GamePlayTime += DeltaSeconds;
			CurStageTime += DeltaSeconds;
			CurRegenTime += DeltaSeconds;

			OnRep_PlayTime();
			LOG_SCREEN_IDX(0, FColor::Blue,
			               "Stage : %d\nStage Time: %.2f\nRegenTime : %.2f\nMiddle Boss Time : %.2f\nFinal Boss Time : %.2f",
			               curStage, CurStageTime, RegenTime, MiddleBossRegenTime, FinalBossRegenTime);
			LOG_SCREEN_IDX(1, FColor::Green, "EXP : %.2f", (float)curExp/(float)ExpInfo.XP * 100);
			LOG_SCREEN_IDX(2, FColor::Red, "RegenCount : %d, CurRegenCount : %d", RegenCount, CurRegenCount);
			if (CurRegenTime >= RegenTime)
			{
				if (RegenCount > CurRegenCount)
				{
					++CurRegenCount;
					ObjectPoolManager->EnemySpawn(CurRegenCount & 1);
				}

				CurRegenTime -= RegenTime;
			}
			if (MiddleBossCount > 0 && CurStageTime >= MiddleBossRegenTime && MiddleBossCount > CurMiddleBossCount)
			{
				ObjectPoolManager->MiddleBossSpawn();
				++MiddleBossCount;
			}

			if (FinalBossCount > 0 && CurStageTime >= FinalBossRegenTime && FinalBossCount > CurFinalBossCount)
			{
				// (X=2064.554874,Y=-2316.554872,Z=115.000000)
			}

			if (CurStageTime >= StageTime)
			{
				NextStage();
				CurStageTime = 0;
			}
		}
	}

	if (EXPDataMap.Num() > 0 && curExp >= ExpInfo.XP)
	{
		++curLevel;
		curExp -= ExpInfo.XP;
		UpdateExpInfo(ExpInfo.ID + 1);
		HUD->SetEXP(0, ExpInfo.XP);
	}
}

void ACGameState::PrintExpDataMap()
{
	// 초기 데이터 세팅
	if (EXPDataMap.Num() > 0)
		UpdateExpInfo(1);
}

void ACGameState::AddExp(const int32 exp)
{
	curExp += exp;
	HUD->SetEXP(curExp, ExpInfo.XP);
}

void ACGameState::NextStage()
{
	++curStage;
	if (OnNextStage.IsBound()) OnNextStage.Broadcast(); // 아이템 박스 생성, 추후 추가 바인딩 가능
	GM->UpdateRegenInfo(curStage); // 리젠 테이블 갱신
}

void ACGameState::CalcEnemyRegenTime(const FRegenData& RegenData)
{
	CurRegenCount = 0;
	RegenTime = StageTime / (float)(RegenData.MeleeEnemyCount + RegenData.RangeEnemyCount);
	RegenCount = RegenData.MeleeEnemyCount + RegenData.RangeEnemyCount;
	MiddleBossCount = RegenData.MidleBossCount;
	MiddleBossRegenTime = StageTime / (float)MiddleBossCount;
	FinalBossCount = RegenData.FinalBossCount;
	FinalBossRegenTime = StageTime / (float)FinalBossCount;
}

void ACGameState::ReadExcelData(class UCDataSheetUtility* Utility)
{
	if (Utility)
	{
		Utility->OnDataFetched.AddDynamic(this, &ACGameState::PrintExpDataMap);
		Utility->FetchGoogleSheetData<FEXPData>("EXP", "A1", "B22", EXPDataMap);
	}
}


void ACGameState::UpdateExpInfo(const int32 Level)
{
	if (Level >= MaxLevel) return;
	const auto& StatData = EXPDataMap[Level];
	ExpInfo.ID = StatData.ID;
	ExpInfo.XP = StatData.XP;

	if (Level == 1) return;
	LOG_S(Warning, TEXT("UpdateExpInfo : %d, %d"), ExpInfo.ID, ExpInfo.XP);
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
	{
		(*It)->UpdatePlayerData(Level);
	}
}

void ACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGameState, GamePlayTime);
	DOREPLIFETIME(ACGameState, UpgradeSelectPlayerCount);
	DOREPLIFETIME(ACGameState, ReadyPlayer);
}

void ACGameState::OnRep_PlayTime()
{
	HUD->SetTime(GamePlayTime);
}

void ACGameState::OnRep_UpgradeSelectPlayerCount()
{
	if (UpgradeSelectPlayerCount < PlayerArray.Num()) return;
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
	{
		(*It)->SRPC_UnPause();
	}
	UpgradeSelectPlayerCount = 0;
}

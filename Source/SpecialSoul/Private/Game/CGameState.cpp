// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameState.h"

#include "EngineUtils.h"
#include "SpecialSoul.h"
#include "Components/WidgetComponent.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "Game/SpecialSoulGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"
#include "UI/OverheadStatusWidget.h"
#include "UI/HUD/GameHUD.h"
#include "Utility/CDataSheetUtility.h"

ACGameState::ACGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor> MainBossClassFinder(
		TEXT("/Game/Enemy/MainBoss/BP_MainBoss.BP_MainBoss_C"));
	if (MainBossClassFinder.Succeeded())
	{
		MainBossClass = MainBossClassFinder.Class;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder(
		TEXT("/Game/Micellaneous/C_TimeDilation.C_TimeDilation"));
	if (CurveFinder.Succeeded())
	{
		TimeDilationCurve = CurveFinder.Object;
	}
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

	// 시간지연(보스 사망 시 사용됨)
	if (TimeDilationCurve)
	{
		FOnTimelineFloat timelineCallback;
		// "UpdateTimeDilation" 함수에 대한 콜백 등록
		timelineCallback.BindUFunction(this, FName("UpdateTimeDilation"));
		TimeDilationTimeline.AddInterpFloat(TimeDilationCurve, timelineCallback);

		// 타임라인 끝난 후 콜백
		FOnTimelineEvent timelineFinishCallback;
		timelineFinishCallback.BindUFunction(this, FName("OnTimeDilationFinished"));
		TimeDilationTimeline.SetTimelineFinishedFunc(timelineFinishCallback);

		TimeDilationTimeline.SetLooping(false);
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
			LOG_SCREEN_IDX(1, FColor::Green, "EXP : %.2f", (float)curExp / (float)ExpInfo.XP * 100);
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
				MiddleBossRegenTime = 0.f;
			}

			if (FinalBossCount > 0 && CurStageTime >= FinalBossRegenTime && FinalBossCount > CurFinalBossCount)
			{
				// 스폰
				//FVector SpawnLocation(2064.554874f, -2316.554872f, 115.0f);
				//FRotator SpawnRotation(0.0f, 0.0f, 0.0f); // 기본 회전
				//FActorSpawnParameters SpawnParams;
				//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				//if (MainBossClass)
				//	{
				//		AMainBoss* SpawnedBoss = GetWorld()->SpawnActor<AMainBoss>(MainBossClass, SpawnLocation, SpawnRotation, SpawnParams);
				//		if (SpawnedBoss)
				//			--FinalBossCount;
				//		else
				//			UE_LOG(LogTemp, Error, TEXT("Failed to spawn BP_MainBoss"));
				//	}
				if (!bMainBossSpawned)
				{
					SpawnMainBoss(FinalBossCount);
					FinalBossRegenTime = 0.f;
				}
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
		UpdateExpUI();
	}

	// 시간 조작
	TimeDilationTimeline.TickTimeline(DeltaSeconds);
}

void ACGameState::OnMainBossDie()
{
	GameEndProcess();
}

void ACGameState::GameEndProcess()
{
	// 천천히 시간 늦추기 -> 게임종료 UI 띄우기
	// UpdateTimeDilation -> OnTimeDilationFinished
	if (TimeDilationCurve)
	{
		TimeDilationTimeline.PlayFromStart();
	}
}

void ACGameState::PlayFail()
{
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if (auto myPC = Cast<ACPlayerController>(*it))
		{
			myPC->CRPC_ShowGameEndingUI(false);
		}
	}
}

void ACGameState::UpdateTimeDilation(float value)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), value);
}

void ACGameState::OnTimeDilationFinished()
{
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if (auto myPC = Cast<ACPlayerController>(*it))
		{
			const bool bWin = !SpawnedBoss || SpawnedBoss->bIsDead;
			myPC->CRPC_ShowGameEndingUI(bWin);
		}
	}
}

void ACGameState::PrintExpDataMap()
{
	// 초기 데이터 세팅
	if (EXPDataMap.Num() > 0)
		UpdateExpInfo(1);
}

void ACGameState::UpdateExpUI()
{
	if (IsValid(HUD))
	{
		HUD->SetEXP(curExp, ExpInfo.XP); // 리슨서버 직접 갱신
	}

	if (HasAuthority())
	{
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (ACPlayerController* PC = Cast<ACPlayerController>(It->Get()))
			{
				if (!PC->IsLocalController()) //서버의 로컬컨트롤러 아니면 (즉, 클라들) 갱신
				{
					PC->CRPC_UpdateExpUI(curExp, ExpInfo.XP);
				}
			}
		}
	}
}

void ACGameState::AddExp(const int32 exp)
{
	curExp += exp;
	UpdateExpUI();
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

	// 서버에서 모든 캐릭터를 갱신
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
	{
		(*It)->UpdatePlayerData(Level);
	}

	if (IsValid(HUD))
	{
		HUD->SetLevel(Level); // 리슨서버 직접 갱신
	}
	if (HasAuthority())
	{
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (ACPlayerController* PC = Cast<ACPlayerController>(It->Get()))
			{
				if (!PC->IsLocalController()) //서버의 로컬컨트롤러 아니면 (즉, 클라들) 갱신
				{
					PC->CRPC_UpdateLevelUI(Level);
				}
			}
		}
	}

	MRPC_UpdateLevelUI(Level);
}

void ACGameState::MRPC_UpdateLevelUI_Implementation(int32 Level)
{
	// 모든 클라이언트에서 Overhead UI 갱신
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
	{
		ACBasePlayer* Player = *It;
		if (Player/* && Player->IsLocallyControlled()*/) // 각 클라이언트에서 갱신
		{
			if (auto OverheadUI = Cast<UOverheadStatusWidget>(Player->OverheadUIComp->GetWidget()))
			{
				OverheadUI->SetLevel(Level); // 레벨 갱신
			}
		}
	}
}

void ACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGameState, GamePlayTime);
	DOREPLIFETIME(ACGameState, UpgradeSelectPlayerCount);
	DOREPLIFETIME(ACGameState, ReadyPlayer);
	DOREPLIFETIME(ACGameState, DeadPlayer);
	DOREPLIFETIME(ACGameState, AlivePlayer);
}

void ACGameState::OnRep_PlayTime()
{
	HUD->SetTime(GamePlayTime);
}

void ACGameState::OnRep_UpgradeSelectPlayerCount()
{
	if (UpgradeSelectPlayerCount < AlivePlayer) return;
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
	{
		(*It)->SRPC_UnPause();
	}
	UpgradeSelectPlayerCount = 0;
}


void ACGameState::SpawnMainBoss(int32& finalBossCount)
{
	if (bMainBossSpawned) return;

	FVector SpawnLocation(2064.554874f, -2316.554872f, 115.0f);
	FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (MainBossClass)
	{
		SpawnedBoss = GetWorld()->SpawnActor<AMainBoss>(MainBossClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedBoss)
		{
			--FinalBossCount;
			bMainBossSpawned = true;

			MRPC_ShowMainBossUI(SpawnedBoss);
			SpawnedBoss->OnMainBossDie.AddDynamic(this, &ACGameState::OnMainBossDie);
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn BP_MainBoss"));
	}
}


// 모든 클라가 메인보스 UI를 띄우도록 요청한다
void ACGameState::MRPC_ShowMainBossUI_Implementation(AMainBoss* spawnedBoss)
{
	auto pc = GetWorld()->GetFirstPlayerController();
	if (!pc) return;
	if (!pc->IsLocalController()) return;

	if (auto myPC = Cast<ACPlayerController>(pc))
	{
		LOG_S(Log, TEXT("MRPC_ShowMainBossUI"));

		myPC->ShowBossUI(spawnedBoss, true);
		myPC->SetBossHPPercent(1.f); // progress bar 꽉 채우기
	}
}

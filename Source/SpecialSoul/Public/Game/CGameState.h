// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecialSoulGameMode.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/GameStateBase.h"
#include "CGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FNextStage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainBossSpawn);

USTRUCT(BlueprintType)
struct FEXPData // 레벨업에 필요한 경험치 테이블
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 XP;

	FEXPData()
		: ID(0), XP(0)
	{
	}

	FEXPData(int32 id, int32 xp)
		: ID(id), XP(xp)
	{
	}
};

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	ACGameState();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	class ASpecialSoulGameMode* GM;
	UPROPERTY()
	TObjectPtr<class ACObjectPoolManager> ObjectPoolManager;
	
	UPROPERTY(EditDefaultsOnly, Category="Game")
	float StageTime = 20.f;
	float CurStageTime = 0.f;

	float RegenTime;
	float CurRegenTime = 0.f;
	int32 RegenCount;
	int32 CurRegenCount;
	int32 MiddleBossCount = 0;
	int32 CurMiddleBossCount = 0;
	float MiddleBossRegenTime = 0.f;
	int32 FinalBossCount = 0;
	int32 CurFinalBossCount = 0;
	float FinalBossRegenTime = 0.f;

public:
	const int32 MaxLevel = 21;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FEXPData> EXPDataMap;

	FNextStage OnNextStage;
	FOnMainBossSpawn OnMainBossSpawn;
	
	UFUNCTION()
	void PrintExpDataMap();
	void UpdateExpUI();
	int32 GetCurLevel() const { return curLevel; }

	void AddExp(const int32 exp);
	void NextStage();

	void CalcEnemyRegenTime(const FRegenData& RegenData);

	void ReadExcelData(class UCDataSheetUtility* Utility);

private:
	UPROPERTY(VisibleAnywhere, Category = "Game")
	int32 curStage = 1;
	UPROPERTY(VisibleAnywhere, Category = "Game")
	int32 curLevel = 1;
	UPROPERTY(VisibleAnywhere, Category = "Game")
	int32 curExp = 0;

	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	FEXPData ExpInfo;

	void UpdateExpInfo(int32 Level);

	UFUNCTION(NetMulticast, Reliable)
	void MRPC_UpdateLevelUI(int32 INT32);
private:
	UPROPERTY()
	class AGameHUD* HUD;
	UPROPERTY(ReplicatedUsing=OnRep_PlayTime)
	float GamePlayTime = 0;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_PlayTime();
public:
	UPROPERTY(ReplicatedUsing = OnRep_UpgradeSelectPlayerCount)
	int32 UpgradeSelectPlayerCount = 0;
	UFUNCTION()
	void OnRep_UpgradeSelectPlayerCount();

public:
	UPROPERTY(Replicated)
	int32 ReadyPlayer=0;
	UPROPERTY(Replicated)
	int32 AlivePlayer=0;

private:
	UPROPERTY(EditAnywhere, Category = "MainBoss")
	TSubclassOf<AActor> MainBossClass;

	bool bMainBossSpawned {false};
	
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_ShowMainBossUI(class AMainBoss* spawnedBoss);

	UFUNCTION()
	void OnMainBossDie();
	
	void SpawnMainBoss(int32& finalBossCount);
	
	UPROPERTY(VisibleAnywhere, Category = "MainBoss")
	class AMainBoss* SpawnedBoss;

private:
	UPROPERTY()
	UCurveFloat* TimeDilationCurve;

	FTimeline TimeDilationTimeline;
	UFUNCTION()
	void UpdateTimeDilation(float value);

	UFUNCTION()
	void OnTimeDilationFinished();

	void GameEndProcess();

public:
	UPROPERTY(Replicated)
	int32 DeadPlayer=0;

	void PlayFail();
};

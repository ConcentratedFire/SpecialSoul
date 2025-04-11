// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CGameState.generated.h"

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

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FEXPData> EXPDataMap;

	UFUNCTION()
	void PrintAttackDataMap();
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Game")
	int32 curLevel = 1;
	UPROPERTY(VisibleAnywhere, Category = "Game")
	float curExp = 0;

	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	FEXPData ExpInfo;
	
	void UpdateYasuoAttackStat(int32 Level);
};

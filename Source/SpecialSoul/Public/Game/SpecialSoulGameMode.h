// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpecialSoulGameMode.generated.h"

USTRUCT(BlueprintType)
struct FRegenData // 레벨업에 필요한 경험치 테이블
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 MeleeEnemyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 RangeEnemyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 MidleBossCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 FinalBossCount;

	FRegenData()
		: ID(0), MeleeEnemyCount(0), RangeEnemyCount(0), MidleBossCount(0), FinalBossCount(0)
	{
	}

	FRegenData(int32 id, int32 meleeEnemyCount, int32 rangeEnemyCount, int32 midleBossCount, int32 finalBossCount)
		: ID(id), MeleeEnemyCount(meleeEnemyCount), RangeEnemyCount(rangeEnemyCount), MidleBossCount(midleBossCount),
		  FinalBossCount(finalBossCount)
	{
	}
};

UCLASS(minimalapi)
class ASpecialSoulGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpecialSoulGameMode();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FRegenData> RegenDataMap;

	UFUNCTION()
	void PrintAttackDataMap();

private:
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	FRegenData RegenInfo;
	
	void UpdateYasuoAttackStat(int32 Level);
};

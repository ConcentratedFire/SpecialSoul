// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeData // 종류별 등급 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString UpgradeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 MaxGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString CaculType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 DefaultValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 AppendValue;

	FUpgradeData()
		: UpgradeType("None"), MaxGrade(0), CaculType("Append"), DefaultValue(0), AppendValue(0)
	{
	}

	FUpgradeData(FString upgradeType, int32 maxGrade, FString caculType, int32 defaultValue, int32 appendValue)
		: UpgradeType(upgradeType), MaxGrade(maxGrade), CaculType(caculType), DefaultValue(defaultValue),
		  AppendValue(appendValue)
	{
	}
};

DECLARE_DELEGATE(FUpdatdStat);

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void InitPlayerState(class UCDataSheetUtility* Utility);

	UFUNCTION()
	void UpdateGradeInfo();

	FORCEINLINE void GetCurrentUpgradeGrade(int32& DamageGrade, int32& AbilityHasteGrade, int32& ProjectilesGrade,
	                                        int32& CritChanceGrade)
	{
		DamageGrade = CurDamageGrade, AbilityHasteGrade = CurAbilityHasteGrade, ProjectilesGrade = CurProjectilesGrade,
			CritChanceGrade = CurCritChanceGrade;
	};

	FORCEINLINE void GetIsMaxUpgrade(bool& IsDamageMax, bool& IsAbilityHasteMax, bool& IsProjectilesMax,
	                                 bool& IsCritChanceMax)
	{
		IsDamageMax = CurDamageGrade == UpgradeDataMap[FString("Damage")].MaxGrade,
			IsAbilityHasteMax = CurAbilityHasteGrade == UpgradeDataMap[FString("AbilityHaste")].MaxGrade,
			IsProjectilesMax = CurProjectilesGrade == UpgradeDataMap[FString("Projectiles")].MaxGrade,
			IsCritChanceMax = CurCritChanceGrade == UpgradeDataMap[FString("CritChance")].MaxGrade;
	}

private:
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TMap<FString, FUpgradeData> UpgradeDataMap;

	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurDamageGrade = 0;
	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurAbilityHasteGrade = 0;
	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurProjectilesGrade = 0;
	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurCritChanceGrade = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecialSoul.h"
#include "Engine/DataTable.h"
#include "CYasuoData.generated.h"

USTRUCT(BlueprintType)
struct FYasuoAttackData // 야스오 기본공격 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 ProjectileCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float ProjectileRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString UseAOE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float AOELifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float AOEDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float AOEDamageCoolTime;

	FYasuoAttackData()
		: ID(0), ProjectileCount(0), ProjectileRange(0), Damage(0), UseAOE("N"), AOELifeTime(0.f), AOEDamage(0.f),
		  AOEDamageCoolTime(0.f)
	{
	}

	FYasuoAttackData(int32 id, int32 projectileCount, float projectileRange, float damage, FString useAOE,
	                 float aoeLifeTime, float aoeDamage, float AOEDamageCoolTime)
		: ID(id), ProjectileCount(projectileCount), ProjectileRange(projectileRange), Damage(damage),
		  UseAOE(useAOE), AOELifeTime(aoeLifeTime), AOEDamage(aoeDamage), AOEDamageCoolTime(AOEDamageCoolTime)
	{
	}
};

USTRUCT(BlueprintType)
struct FYasuoMoveData // 야스오 이동거리 기류 획득 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 RangeFrom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 RangeTo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	float StackDistance;

	FYasuoMoveData()
		: ID(0), RangeFrom(0), RangeTo(0), StackDistance(0)
	{
	}

	FYasuoMoveData(int32 id, int32 rangeFrom, float rangeTo, float stackDistance)
		: ID(id), RangeFrom(rangeFrom), RangeTo(rangeTo), StackDistance(stackDistance)
	{
	}
};

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FCYasuoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	uint8 ProjectileCount;
	UPROPERTY(EditAnywhere)
	int32 ProjectileRange;
	UPROPERTY(EditAnywhere)
	int32 Damage;
	UPROPERTY(EditAnywhere)
	uint8 UseAOE;
	UPROPERTY(EditAnywhere)
	float AOELifeTime;
	UPROPERTY(EditAnywhere)
	float AOEDamage;
	UPROPERTY(EditAnywhere)
	float AOEDamageCoolTime;
};

USTRUCT(BlueprintType)
struct FCardStruct
{
	GENERATED_BODY()

	bool bIsNextWeaponFinal;
	bool bIsYasuo;
	int32 WeaponGrade;
	int32 DamageGrade;
	int32 AbilityHasteGrade;
	int32 ProjectilesGrade;
	int32 CritChanceGrade;
	int32 curDamage;
	int32 nextDamage;
	int32 curProjectile;
	int32 nextProjectile;
	FString strLevel;
	FString strTitle;
	FString strDesc;
	FString strStat;
	FString strDesc2;
	FString strStat2;
	FString strUpgradeStat;

	FCardStruct(): bIsNextWeaponFinal(false), bIsYasuo(false), WeaponGrade(0), DamageGrade(0), AbilityHasteGrade(0),
	               ProjectilesGrade(0), CritChanceGrade(0), curDamage(0), nextDamage(0), curProjectile(0),
	               nextProjectile(0),
	               strLevel(""), strTitle(""), strDesc(""), strStat(""), strDesc2(""), strStat2(""), strUpgradeStat("")
	{
	}

	void Print_Log()
	{
		UE_LOG(LogTemp, Warning,TEXT(
				  "\nbIsNextWeaponFinal: %d, bIsYasuo: %d\nWeaponGrade: %d, DamageGrade: %d, AbilityHasteGrade: %d, ProjectilesGrade: %d\n"
				  "CritChanceGrade: %d, curDamage: %d, nextDamage: %d, curProjectile: %d, nextProjectile: %d\n"
				  "strLevel: %s, strTitle: %s, strDesc: %s, strStat: %s\nstrDesc2: %s, strStat2: %s, strUpgradeStat: %s"),
				  bIsNextWeaponFinal, bIsYasuo, WeaponGrade, DamageGrade, AbilityHasteGrade, ProjectilesGrade, CritChanceGrade,curDamage, nextDamage, curProjectile, nextProjectile,
				  *strLevel, *strTitle, *strDesc, *strStat, *strDesc2, *strStat2, *strUpgradeStat);
	}
};

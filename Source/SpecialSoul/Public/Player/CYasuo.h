// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CBasePlayer.h"
#include "CYasuo.generated.h"


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

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACYasuo : public ACBasePlayer
{
	GENERATED_BODY()

private:
	ACYasuo();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FYasuoAttackData> AttackDataMap;

	virtual void Attack() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	FYasuoAttackData YasuoStat;
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	int32 PassiveEnergy = 0;

private: // Attack
	UPROPERTY(EditDefaultsOnly, category=Attack)
	TSubclassOf<class ACTornado> TornadoFactory;
	UPROPERTY(VisibleAnywhere, Category=Attack)
	int32 AttackCnt = 5;

	TArray<FVector> GetAttackVector();

private: // Anim
	UPROPERTY()
	class UCYasuoAnim* Anim;

private: // Spec
	UFUNCTION()
	void PrintAttackDataMap(); // 임시

	UPROPERTY()
	class UCDataSheetUtility* DataSheetUtility;

	void UpdateYasuoStat(const int32 Level);

private: // Passive Energy
	const int32 PassiveEnergyMax = 100;
	const int32 PassiveEnergyRegen = 4;
	FTimerHandle ChargePassiveEnergyTimer;

	UFUNCTION()
	void ChargePassiveEnergy();
};

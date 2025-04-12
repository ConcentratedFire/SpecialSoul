#pragma once

#include "CoreMinimal.h"
#include "JinxData.generated.h" 

USTRUCT(BlueprintType)
struct FJinxAttackData // 징크스 기본공격 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32	ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32	ProjectileCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	ProjectileRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	Cooltime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString	UseAP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	APDamage;

	FJinxAttackData()
		: ID(0), ProjectileCount(0), ProjectileRange(0), Damage(0), Cooltime(0), UseAP("N"), APDamage(0.0f)
	{}
	
	FJinxAttackData(int32 id, int32 projectileCount, float projectileRange, float damage, float cooltime, FString useAP, float apDamage )
		: ID(id), ProjectileCount(projectileCount), ProjectileRange(projectileRange), Damage(damage), Cooltime(cooltime),UseAP(useAP), APDamage(apDamage)
	{}
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Player/CBasePlayer.h"
#include "Jinx.generated.h"

class UJinxAnim;
class ISkillStrategy;

UENUM(BlueprintType)
enum class ESkillKey : uint8
{
	Attack, Passive, E, R
};

USTRUCT(BlueprintType)
struct FJinxAttackData // 징크스 기본공격 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32	ProjectileCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	ProjectileRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	Cooltime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	bool	bUseAP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float	APDamage;

	FJinxAttackData(){}
	
	FJinxAttackData(int32 projectileCount, float projectileRange, float damage, float cooltime, bool buseAP, float apDamage )
		: ProjectileCount(projectileCount), ProjectileRange(projectileRange), Damage(damage), Cooltime(cooltime),bUseAP(buseAP), APDamage(apDamage)
	{}
};

UCLASS()
class SPECIALSOUL_API AJinx : public ACBasePlayer
{
	GENERATED_BODY()

public:
	AJinx();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jinx")
	TObjectPtr<UJinxAnim> Anim;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FJinxAttackData AttackData; // 기본공격 데이터
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, TScriptInterface<ISkillStrategy>> SkillMap;

	void CastSkill(ESkillKey Key);
	
private:
	void BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill);

	FTimerHandle AttackTimer;
	void StartAttack();
};

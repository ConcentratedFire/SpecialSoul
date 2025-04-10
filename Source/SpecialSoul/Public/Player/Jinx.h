// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Player/CBasePlayer.h"
#include "Jinx.generated.h"

class UCDataSheetUtility;
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

UCLASS()
class SPECIALSOUL_API AJinx : public ACBasePlayer
{
	GENERATED_BODY()

public:
	AJinx();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jinx")
	TObjectPtr<UJinxAnim> Anim;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FJinxAttackData AttackData; // 기본공격 데이터

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FJinxAttackData> AttackDataMap;
	TSharedPtr<TMap<int32, FJinxAttackData>> SafeMap = MakeShared<TMap<int32, FJinxAttackData>>();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, TScriptInterface<ISkillStrategy>> SkillMap;

	void CastSkill(ESkillKey Key);

	UFUNCTION()
	void SetAttackData(); // 임시
	
	UFUNCTION()
	void PrintAttackDataMap(); // 임시
	
private:

	UPROPERTY()
	UCDataSheetUtility* DataSheetUtility;
	
	void BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill);

	FTimerHandle AttackTimer;
	void StartAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;
	
};


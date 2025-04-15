// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/JinxData.h"
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

// 스킬 사용 중에는 방향 회전 막기


UCLASS()
class SPECIALSOUL_API AJinx : public ACBasePlayer
{
	GENERATED_BODY()

public:
	AJinx();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void PossessedBy(AController* NewController) override;

	virtual void Attack() override;
	UFUNCTION()
	void InitAllData();
	virtual void UpdatePlayerData(const int32 PlayerLevel) override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jinx")
	TObjectPtr<UJinxAnim> Anim;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FJinxAttackData AttackData; // 기본공격 데이터

	TSharedPtr<TMap<int32, FJinxAttackData>> SafeMap = MakeShared<TMap<int32, FJinxAttackData>>();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, TScriptInterface<ISkillStrategy>> SkillMap;

	void CastSkill(ESkillKey Key);

	UFUNCTION()
	virtual void PrintAttackDataMap() override;
	
private:
	void UpdateJinxAttackStat(int32 PlayerLevel);

	void BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill);

	FTimerHandle AttackTimer;
	void StartAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;
	
};


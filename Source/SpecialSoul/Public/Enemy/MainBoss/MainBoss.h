// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "MainBoss.generated.h"

class USkillComponent;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API AMainBoss : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AMainBoss();
	virtual void DieEndAction() override;
	void ResetLeftCooltime_DarkinBlade();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void PlayDarkinBladeMontage(float InPlayRate, FName SectionName);

public: // ==== 몽타쥬 ===========
	UPROPERTY(EditDefaultsOnly, Category = "MainBoss")
	TObjectPtr<UAnimMontage> DarkinBlade_Montage; // Q스킬, 3연 공격

	UPROPERTY(VisibleAnywhere, Category = "MainBoss")
	TObjectPtr<USkillComponent> SkillComponent;

	virtual void MyDamage(int32 DamageAmount) override;
	
	void ChangePhase();
	
	float GetHpPercent() const { return HP / MaxHP; }

	UPROPERTY(VisibleAnywhere, Category = "MainBoss")
	bool bIsUlt {false}; // 궁 쓴 상태
	
};

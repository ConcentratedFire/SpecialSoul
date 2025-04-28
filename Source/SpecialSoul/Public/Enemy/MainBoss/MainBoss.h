// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "MainBoss.generated.h"

class UBoxComponent;
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
	virtual void PossessedBy(AController* NewController) override;
	virtual void HandleDie() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Hitbox)
	UBoxComponent* BladeHitbox;
	//int32 curDarkinBladeComboCount;

	UFUNCTION()
	void OnBladeHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage {100.f}; // 데미지
	
public:
	UFUNCTION(Server, Reliable)
	void SRPC_PlayDarkinBladeMontage(float InPlayRate, FName SectionName);
	
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_PlayDarkinBladeMontage(float InPlayRate, FName SectionName);

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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

enum class EEnemyState : uint8;
class UEnemyAnimInstance;
class UEnemyFSMComponent;

UCLASS()
class SPECIALSOUL_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	
public:
	UPROPERTY()
	class ACEnemyController* MyController;
	UPROPERTY(EditDefaultsOnly, Category = "AI|MoveDistance")
	float MoveDistance {100.f};
	
	UFUNCTION()
	virtual void HandleAttack();

	// UFUNCTION()
	// virtual void HandleDamaged();
	
	UFUNCTION()
	virtual void HandleDie();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseEnemy")
	bool bIsAttacking {false};

	virtual void ResetEnemy();

public:
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }

public: // Damage
	UFUNCTION(BlueprintCallable)
	void MyDamage(int32 DamageAmount);

	virtual void DieEndAction()
	{
	}; // 사망 애니메이션 종료 후, Pool로 돌아가도록 처리

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	TObjectPtr<AActor> Target;

	FTimerHandle FindTargetTimerHandle; // 가장 가까운 타겟을 찾는 Timer

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	float FindTargetInterval{2.f};

	void StartFindingTarget();
	virtual void FindTarget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UEnemyAnimInstance> AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> DieMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Move")
	float MoveSpeed{200.f};

	UPROPERTY(EditDefaultsOnly, category = "HP")
	int32 MaxHP{200};

	int32 HP;

private: // Montage CallBack
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMyControllerTickOn();

	UFUNCTION()
	void OnMyControllerTickOff();

protected:
	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	bool bIsDead{false};
};

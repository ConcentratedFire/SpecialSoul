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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY()
	class ACEnemyController* MyController;
	UPROPERTY(EditDefaultsOnly, Category = "AI|MoveDistance")
	float MoveDistance{100.f};

	UFUNCTION()
	virtual void HandleAttack();

	// UFUNCTION()
	// virtual void HandleDamaged();

	UFUNCTION()
	virtual void HandleDie();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseEnemy")
	bool bIsAttacking{false};

	virtual void ResetEnemy();

public:
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }

public: // Damage
	UFUNCTION(BlueprintCallable)
	void MyDamage(int32 DamageAmount);

	virtual void DieEndAction()
	{
	}; // 사망 애니메이션 종료 후, Pool로 돌아가도록 처리

public: // Speed
	float GetMoveSpeed() const { return MoveSpeed; }

protected:
	virtual void BeginPlay() override;\

public:
	virtual void SetActorTickEnabled(bool bEnabled) override;

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

	UPROPERTY(VisibleAnywhere, Category=HP, Replicated)
	int32 HP;

private: // Montage CallBack
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy", Replicated)
	bool bIsDead{false};

public:
	bool GetIsPlayerInRange(const float Range) const;

private:
	UFUNCTION(Server, Reliable)
	void SRPC_PlayAttackAnim();
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_PlayAttackAnim();
	UFUNCTION(Server, Reliable)
	void SRPC_Damage(int32 DamageAmount);
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_Die();
};

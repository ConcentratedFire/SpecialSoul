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
	
protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	TObjectPtr<AActor> Target;

	FTimerHandle FindTargetTimerHandle; // 가장 가까운 타겟을 찾는 Timer

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	float FindTargetInterval {2.f};

	void StartFindingTarget();
	void FindTarget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UEnemyAnimInstance> AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> DieMontage;

private: // Montage CallBack
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};


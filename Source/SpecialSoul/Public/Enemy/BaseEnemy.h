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

protected:
	virtual void BeginPlay() override;

protected:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	TObjectPtr<UEnemyFSMComponent> FSMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UEnemyAnimInstance> AnimInstance;

	UFUNCTION()
	void OnFSMStateChanged(EEnemyState NewState);

	UFUNCTION()
	virtual void HandleAttack();

	// UFUNCTION()
	// virtual void HandleDamaged();
	
	UFUNCTION()
	virtual void HandleDie();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> DieMontage;
};


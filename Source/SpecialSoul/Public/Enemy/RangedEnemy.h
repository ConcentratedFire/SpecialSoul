// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "RangedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ARangedEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ARangedEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	TObjectPtr<UEnemyFSMComponent> FSMComponent;


	UFUNCTION()
	void OnFSMStateChanged(EEnemyState NewState);

};

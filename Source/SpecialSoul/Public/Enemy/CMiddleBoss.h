// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "CMiddleBoss.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACMiddleBoss : public ABaseEnemy
{
	GENERATED_BODY()

private:
	ACMiddleBoss();

public:
	void Attack();

	virtual void DieEndAction() override;
};

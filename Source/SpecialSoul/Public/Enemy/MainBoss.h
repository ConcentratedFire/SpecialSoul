// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "MainBoss.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API AMainBoss : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AMainBoss();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};

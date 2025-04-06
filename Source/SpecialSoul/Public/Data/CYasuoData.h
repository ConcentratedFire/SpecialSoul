// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CYasuoData.generated.h"

/**
 * 
 */
USTRUCT(Atomic,BlueprintType)
struct FCYasuoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	uint8 ProjectileCount;
	UPROPERTY(EditAnywhere)
	int32 ProjectileRange;
	UPROPERTY(EditAnywhere)
	int32 Damage;
	UPROPERTY(EditAnywhere)
	uint8 UseAOE;
	UPROPERTY(EditAnywhere)
	float AOELifeTime;
	UPROPERTY(EditAnywhere)
	float AOEDamage;
	UPROPERTY(EditAnywhere)
	float AOEDamageCoolTime;
};

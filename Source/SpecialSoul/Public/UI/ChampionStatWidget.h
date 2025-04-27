// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChampionStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UChampionStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDamageUpgrade(int32 damageUpgrade);
	void SetCooltimeUpgrade(int32 cooltimeUpgrade);
	void SetCriticalPercent(int32 criticalPercent);
	void SetProjectileCount(int32 projectileCount);

private:
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_DamageUpgrade;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_CooltimeUpgrade;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_CriticalPercent;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_ProjectileCount;
};

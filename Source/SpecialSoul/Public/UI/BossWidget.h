// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "BossWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

public:
	void SetHPBar(float hpPercent);
	void ShowBossUI(AMainBoss* MainBoss, bool bShow);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* IMG_BG;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Txt_Level;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* IMG_UpgradeTarget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Txt_TargetName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Txt_TargetDesc;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Txt_UpgradeStat;
};

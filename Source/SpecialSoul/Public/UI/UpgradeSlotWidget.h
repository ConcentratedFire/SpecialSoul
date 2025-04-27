// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UUpgradeSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUpgradeCount(int count);
	void SetImageIcon(UTexture* ImageTexture);
	

private:
	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UTextBlock* Text_UpgradeCount;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UImage* Image_Icon;
};

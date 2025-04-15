// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

public:
	FString CardName = "";

	void SetBg(UTexture* BG)
	{
		IMG_BG->SetBrushFromTexture(Cast<UTexture2D>(BG), false);
	};

	void SetTarget(const FString Level, UTexture* Target, const FString TargetName, const FString TargetDesc,
	               const FString UpgradeStat, const FString TargetDesc2 = "",
	               const FString UpgradeStat2 = "")
	{
		Txt_Level->SetText(FText::FromString(Level));
		IMG_UpgradeTarget->SetBrushFromTexture(Cast<UTexture2D>(Target), false);
		Txt_TargetName->SetText(FText::FromString(TargetName));
		Txt_TargetDesc->SetText(FText::FromString(TargetDesc));
		Txt_UpgradeStat->SetText(FText::FromString(UpgradeStat));
		Txt_TargetDesc2->SetText(FText::FromString(TargetDesc2));
		Txt_UpgradeStat2->SetText(FText::FromString(UpgradeStat2));
	};

private:
	UPROPERTY()
	class ACPlayerState* PlayerState;

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
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Txt_TargetDesc2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Txt_UpgradeStat2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Select;

	UFUNCTION()
	void OnSelect();
};

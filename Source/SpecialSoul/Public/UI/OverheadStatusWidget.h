// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UOverheadStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HP)
	float HP {1.f};
	void SetHP(float hp, float maxHP);
	void SetEnergy(float energy, float maxEnergy);
	void SetLevel(int32 level);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HP_Bar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Energy_Bar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Level;
};
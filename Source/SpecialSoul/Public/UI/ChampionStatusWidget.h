// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChampionStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UChampionStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_E;

	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_R;
};

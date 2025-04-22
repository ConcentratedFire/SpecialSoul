// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/Components/SkillComponent.h"
#include "GameWidget.generated.h"

enum class ESkillKey : uint8;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetKillScore(int32 KillScore);
	void SetTime(float Time);

	void UpdateSkillCooltime(ESkillKey skillKey, FSkillCooltime cooltimeInfo);
	void SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject);
	
	void SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging);
	void ChangeChargeCount(ESkillKey skillKey, int32 count);

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UCKillScoreWidget* WBP_KillScore;
	
	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UCTimeWidget* WBP_Timer;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UChampionStatusWidget* WBP_ChampionStatusWidget;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UChampionStatWidget* WBP_ChampionStatWidget;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UOverheadStatusWidget* WBP_OverheadStatusWidget;
};

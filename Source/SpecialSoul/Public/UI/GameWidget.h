// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

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
	
private:
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

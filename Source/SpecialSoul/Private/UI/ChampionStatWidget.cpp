// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChampionStatWidget.h"
#include "Components/TextBlock.h"

void UChampionStatWidget::SetDamageUpgrade(int32 damageUpgrade)
{
	FString txt = FString::Printf(TEXT("%d%%"), damageUpgrade);
	txt_DamageUpgrade->SetText(FText::FromString(txt));
}

void UChampionStatWidget::SetCooltimeUpgrade(int32 cooltimeUpgrade)
{
	FString txt = FString::Printf(TEXT("%d"), cooltimeUpgrade);
	txt_CooltimeUpgrade->SetText(FText::FromString(txt));
}

void UChampionStatWidget::SetCriticalPercent(int32 criticalPercent)
{
		FString txt = FString::Printf(TEXT("%d"), criticalPercent);
    	txt_CriticalPercent->SetText(FText::FromString(txt));
}

void UChampionStatWidget::SetProjectileCount(int32 projectileCount)
{
	FString txt = FString::Printf(TEXT("%d"), projectileCount);
	txt_ProjectileCount->SetText(FText::FromString(txt));
}

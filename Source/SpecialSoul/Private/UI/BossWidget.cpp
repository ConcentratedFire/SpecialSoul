// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossWidget.h"

#include "Components/ProgressBar.h"

void UBossWidget::SetHPBar(float hpPercent)
{
	if (HPBar)
		HPBar->SetPercent(hpPercent);
}

void UBossWidget::ShowBossUI(AMainBoss* MainBoss, bool bShow)
{
	if (!HPBar) return;

	if (bShow)
		HPBar->SetVisibility(ESlateVisibility::Visible);
	else
		HPBar->SetVisibility(ESlateVisibility::Hidden);
}

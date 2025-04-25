// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OverheadStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UOverheadStatusWidget::ChangeHP(float hp, float maxHP)
{
	HP_Bar->SetPercent(hp/maxHP);
}

void UOverheadStatusWidget::ChangeEnergy(float energy, float maxEnergy)
{
	Energy_Bar->SetPercent(energy/maxEnergy);
}

void UOverheadStatusWidget::SetLevel(int32 level)
{
	Text_Level->SetText(FText::AsNumber(level));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameWidget.h"

#include "UI/HUD/CKillScoreWidget.h"
#include "UI/HUD/CTimeWidget.h"

void UGameWidget::SetKillScore(int32 KillScore)
{
	WBP_KillScore->SetKillScore(KillScore);
}

void UGameWidget::SetTime(float Time)
{
	WBP_Timer->SetTime(Time);
}

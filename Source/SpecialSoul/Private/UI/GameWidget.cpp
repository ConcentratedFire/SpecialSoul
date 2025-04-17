// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameWidget.h"

#include "UI/HUD/CKillScoreWidget.h"

void UGameWidget::SetKillScore(int32 KillScore)
{
	WBP_KillScore->SetKillScore(KillScore);
}

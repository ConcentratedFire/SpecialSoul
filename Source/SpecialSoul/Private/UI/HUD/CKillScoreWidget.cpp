// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/CKillScoreWidget.h"

#include "Components/TextBlock.h"

void UCKillScoreWidget::SetKillScore(int32 KillScore)
{
	TxtKillScore->SetText(FText::AsNumber(KillScore));
}

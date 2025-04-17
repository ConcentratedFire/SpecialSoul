// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/GameHUD.h"
#include "UI/GameWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (GameWidgetClass)
	{
		GameWidget = CreateWidget<UGameWidget>(GetWorld(), GameWidgetClass);
		if (GameWidget)
		{
			GameWidget->AddToViewport();
			GameWidget->SetKillScore(0);
		}
	}
}

void AGameHUD::SetKillScore(int32 KillScore)
{
	if (GameWidget)
		GameWidget->SetKillScore(KillScore);
}

void AGameHUD::SetTime(float Time)
{
	if (GameWidget)
		GameWidget->SetTime(Time);
}

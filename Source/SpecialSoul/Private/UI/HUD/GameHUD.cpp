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
			GameWidget->SetTime(0);
			GameWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AGameHUD::ShowWidget()
{
	if (GameWidget)
		GameWidget->SetVisibility(ESlateVisibility::Visible);
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

void AGameHUD::UpdateSkillCooltime(ESkillKey skillKey, FSkillCooltime cooltimeInfo)
{
	GameWidget->UpdateSkillCooltime(skillKey, cooltimeInfo);
}

void AGameHUD::SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap)
{
	GameWidget->SetMiniMapTexture(TX_MiniMap);
}

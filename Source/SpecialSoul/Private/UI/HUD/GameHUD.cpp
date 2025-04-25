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
		}
	}
}

void AGameHUD::ChangeHP(float HP, float MaxHP)
{
	if (GameWidget)
		GameWidget->ChangeHP(HP, MaxHP);
}

void AGameHUD::SetSkillSlotVisuals(ESkillKey SkillKey, UObject* Object)
{
	if (GameWidget)
		GameWidget->SetSkillSlotVisuals(SkillKey, Object);
}

void AGameHUD::SetPortrait(UObject* Object)
{
	if (GameWidget)
		GameWidget->SetPortrait(Object);
}

void AGameHUD::SetPassiveImage(UObject* Object)
{
	if (GameWidget)
		GameWidget->SetPassiveImage(Object);
}

void AGameHUD::SetPassiveText(int32 count)
{
	if (GameWidget)
		GameWidget->SetPassiveText(count);
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
	if (GameWidget)
		GameWidget->UpdateSkillCooltime(skillKey, cooltimeInfo);
}

void AGameHUD::SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap)
{
	if (GameWidget)
		GameWidget->SetMiniMapTexture(TX_MiniMap);
}

void AGameHUD::SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging)
{
	if (GameWidget)
		GameWidget->SetSkillSlotIsCharging(skillKey, bIsCharging);
}

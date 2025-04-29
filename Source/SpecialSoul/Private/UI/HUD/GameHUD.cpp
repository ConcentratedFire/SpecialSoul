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

void AGameHUD::SetUpgradeSlot(FString upgradeName, int32 upgradeCount)
{
	if (GameWidget)
		GameWidget->SetUpgradeSlot(upgradeName, upgradeCount);
}

void AGameHUD::SetHP(float HP, float MaxHP)
{
	if (GameWidget)
		GameWidget->SetHP(HP, MaxHP);
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

void AGameHUD::SetLevel(int32 level)
{
	if (GameWidget)
		GameWidget->SetLevel(level);
}

void AGameHUD::SetEnergy(float energy, float maxEnergy)
{
	if (GameWidget)
		GameWidget->SetEnergy(energy, maxEnergy);
	
}

void AGameHUD::SetEXP(float exp, float maxExp)
{
	if (GameWidget)
		GameWidget->SetEXP(exp, maxExp);
}

void AGameHUD::SetDefaultWeaponUI(ACBasePlayer* player)
{
	if (GameWidget)
		GameWidget->SetDefaultWeaponUI(player);
}

void AGameHUD::SetBossHPBarPercent(float hpPercent)
{
	if (GameWidget)
		GameWidget->SetBossHPBar(hpPercent);
}

void AGameHUD::ShowBossUI(AMainBoss* mainBoss, bool bShow)
{
	if (GameWidget)
		GameWidget->ShowBossUI(mainBoss, bShow);
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameWidget.h"

#include "UI/ChampionStatusWidget.h"
#include "UI/OverheadStatusWidget.h"
#include "UI/HUD/CKillScoreWidget.h"
#include "UI/HUD/CTimeWidget.h"
#include "UI/MiniMap/CMiniMapWidget.h"

void UGameWidget::SetKillScore(int32 KillScore)
{
	WBP_KillScore->SetKillScore(KillScore);
	
	// WBP_ChampionStatusWidget->SetKillScore(KillScore); 징크스
}

void UGameWidget::SetTime(float Time)
{
	WBP_Timer->SetTime(Time);
}

void UGameWidget::UpdateSkillCooltime(ESkillKey skillKey, FSkillCooltime cooltimeInfo)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->UpdateSkillCoolTime(skillKey, cooltimeInfo);
}

void UGameWidget::SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetSkillSlotVisuals(skillKey, InResoueceObject);
}

void UGameWidget::SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetSkillSlotIsCharging(skillKey, bIsCharging);
}

void UGameWidget::ChangeChargeCount(ESkillKey skillKey, int32 count)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->ChangeChargeCount(skillKey, count);
}

void UGameWidget::SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap)
{
	WBP_MiniMap->SetMiniMapTexture(TX_MiniMap);
}

void UGameWidget::ChangeHP(float HP, float MaxHP)
{
	if (WBP_OverheadStatusWidget)
		WBP_OverheadStatusWidget->ChangeHP(HP, MaxHP);
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->ChangeHP(HP, MaxHP);
}

void UGameWidget::ChangeEnergy(float energy, float maxEnergy)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->ChangeEnergy(energy, maxEnergy);

	if (WBP_OverheadStatusWidget)
		WBP_OverheadStatusWidget->ChangeEnergy(energy, maxEnergy);
}

void UGameWidget::ChangeEXP(float exp, float maxExp)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->ChangeEXP(exp, maxExp);
}

void UGameWidget::SetLevel(int32 level)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetLevel(level);
	
	if (WBP_OverheadStatusWidget)
		WBP_OverheadStatusWidget->SetLevel(level);
}


void UGameWidget::SetPortrait(UObject* Object)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetPortrait(Object);
}

void UGameWidget::SetPassiveImage(UObject* Object)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetPassiveImage(Object);
}

void UGameWidget::SetPassiveText(int32 count)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetPassiveText(count);
}

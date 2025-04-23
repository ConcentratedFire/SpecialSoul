// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameWidget.h"

#include "UI/ChampionStatusWidget.h"
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
	WBP_ChampionStatusWidget->UpdateSkillCoolTime(skillKey, cooltimeInfo);
}

void UGameWidget::SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject)
{
	WBP_ChampionStatusWidget->SetSkillSlotVisuals(skillKey, InResoueceObject);
}

void UGameWidget::SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging)
{
	WBP_ChampionStatusWidget->SetSkillSlotIsCharging(skillKey, bIsCharging);
}

void UGameWidget::ChangeChargeCount(ESkillKey skillKey, int32 count)
{
	WBP_ChampionStatusWidget->ChangeChargeCount(skillKey, count);
}

void UGameWidget::SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap)
{
	WBP_MiniMap->SetMiniMapTexture(TX_MiniMap);
}


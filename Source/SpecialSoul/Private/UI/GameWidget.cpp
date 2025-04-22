// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameWidget.h"

#include "UI/ChampionStatusWidget.h"
#include "UI/HUD/CKillScoreWidget.h"
#include "UI/HUD/CTimeWidget.h"

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



// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameWidget.h"

#include "UI/BossWidget.h"
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
		WBP_ChampionStatusWidget->SetChargeCount(skillKey, count);
}

void UGameWidget::SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap)
{
	WBP_MiniMap->SetMiniMapTexture(TX_MiniMap);
}

void UGameWidget::SetHP(float HP, float MaxHP)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetHP(HP, MaxHP);
}

void UGameWidget::SetEnergy(float energy, float maxEnergy)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetEnergy(energy, maxEnergy);
}

void UGameWidget::SetEXP(float exp, float maxExp)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetEXP(exp, maxExp);
}

void UGameWidget::SetLevel(int32 level)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetLevel(level);
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

void UGameWidget::SetUpgradeSlot(FString upgradeName, int32 upgradeCount)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetUpgradeSlot(upgradeName, upgradeCount);
}

void UGameWidget::SetDefaultWeaponUI(ACBasePlayer* player)
{
	if (WBP_ChampionStatusWidget)
		WBP_ChampionStatusWidget->SetDefaultWeaponUI(player);
}

void UGameWidget::SetBossHPBar(float hpPercent)
{
	if (WBP_BossWidget)
		WBP_BossWidget->SetHPBar(hpPercent);
}

void UGameWidget::ShowBossUI(AMainBoss* mainBoss, bool bShow)
{
	if (WBP_BossWidget)
		WBP_BossWidget->ShowBossUI(mainBoss, bShow);
}


// ==== 스탯 UI ====
void UGameWidget::SetStat_DamageUpgrade(int32 damageUpgrade)
{
	WBP_ChampionStatWidget->SetDamageUpgrade(damageUpgrade);
}

void UGameWidget::SetStat_CooltimeUpgrade(int32 cooltimeUpgrade)
{
	WBP_ChampionStatWidget->SetCooltimeUpgrade(cooltimeUpgrade);
}

void UGameWidget::SetStat_CriticalPercent(int32 criticalPercent)
{
	WBP_ChampionStatWidget->SetCriticalPercent(criticalPercent);
}

void UGameWidget::SetStat_ProjectileCount(int32 projectileCount)
{
	WBP_ChampionStatWidget->SetProjectileCount(projectileCount);
}

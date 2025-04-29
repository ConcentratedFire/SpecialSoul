// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChampionStatWidget.h"
#include "Blueprint/UserWidget.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "Player/Components/SkillComponent.h"
#include "GameWidget.generated.h"

class ACBasePlayer;
enum class ESkillKey : uint8;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetKillScore(int32 KillScore);
	void SetTime(float Time);

	void UpdateSkillCooltime(ESkillKey skillKey, FSkillCooltime cooltimeInfo);
	void SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject);
	
	void SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging);
	void ChangeChargeCount(ESkillKey skillKey, int32 count);

	void SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap);
	void SetHP(float HP, float MaxHP);
	void SetEnergy(float energy, float maxEnergy);
	void SetEXP(float exp, float maxExp);
	void SetLevel(int32 level);
	void SetPortrait(UObject* Object);
	void SetPassiveImage(UObject* Object);
	void SetPassiveText(int32 count);

public:
	void SetUpgradeSlot(FString upgradeName, int32 upgradeCount);

public:
	void SetStat_DamageUpgrade(int32 damageUpgrade);
	void SetStat_CooltimeUpgrade(int32 cooltimeUpgrade);
	void SetStat_CriticalPercent(int32 criticalPercent);
	void SetStat_ProjectileCount(int32 projectileCount);
	void SetDefaultWeaponUI(ACBasePlayer* player);
	void SetBossHPBar(float hpPercent);
	void ShowBossUI(AMainBoss* mainBoss, bool bShow);

private:
	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UCKillScoreWidget* WBP_KillScore;
	
	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UCTimeWidget* WBP_Timer;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UChampionStatusWidget* WBP_ChampionStatusWidget;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UChampionStatWidget* WBP_ChampionStatWidget;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UCMiniMapWidget* WBP_MiniMap;

	UPROPERTY(EditDefaultsOnly, meta=(bindwidget))
	class UBossWidget* WBP_BossWidget;
};

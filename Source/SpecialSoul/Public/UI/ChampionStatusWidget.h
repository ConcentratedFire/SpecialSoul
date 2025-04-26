// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/CBasePlayer.h"
#include "Player/Components/SkillComponent.h"
#include "ChampionStatusWidget.generated.h"

class USkillSlotWidget;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UChampionStatusWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateSkillCoolTime(ESkillKey skillKey, FSkillCooltime cooltimeInfo);
	void SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject);
	void SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging);
	void SetChargeCount(ESkillKey skillKey, int32 count);
	void SetHP(float hp, float maxHP);
	void SetEnergy(float energy, float maxEnergy);
	void SetEXP(float exp, float maxExp);
	void SetLevel(int32 level);
	void SetPortrait(UObject* Object);
	void SetPassiveImage(UObject* Object);
	void SetPassiveText(int32 count);

private:
	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_E;

	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_R;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HP_Bar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_HP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Energy_Bar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Energy;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* EXP_Bar;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Level;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Level;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Portrait;
	
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Passive;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_KillCount;
	
private:
	TMap<ESkillKey, USkillSlotWidget*> SkillSlotMap;

};

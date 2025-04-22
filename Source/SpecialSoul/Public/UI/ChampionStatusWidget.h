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
	void ChangeChargeCount(ESkillKey skillKey, int32 count);

	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_E;

	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_R;
private:
	TMap<ESkillKey, USkillSlotWidget*> SkillSlotMap;

};

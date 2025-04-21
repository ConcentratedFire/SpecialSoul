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

private:
	TMap<ESkillKey, USkillSlotWidget*> SkillSlotMap;

	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_E;

	UPROPERTY(meta = (BindWidget))
	class USkillSlotWidget* SkillSlot_R;
};

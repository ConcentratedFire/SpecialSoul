// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChampionStatusWidget.h"

#include "UI/SkillSlotWidget.h"

void UChampionStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!SkillSlot_E)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillSlot_E is nullptr"));
		return;
	}
	
	if (!SkillSlot_R)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillSlot_R is nullptr"));
		return;
	}
	
	SkillSlotMap.Add(ESkillKey::E, SkillSlot_E);
	SkillSlotMap.Add(ESkillKey::R, SkillSlot_R);
}

void UChampionStatusWidget::UpdateSkillCoolTime(ESkillKey skillKey, FSkillCooltime cooltimeInfo)
{
	if (auto slot = SkillSlotMap.Find(skillKey))
		(*slot)->UpdateCoolTime(cooltimeInfo.LeftCooltime, cooltimeInfo.TotalCooltime);
}

void UChampionStatusWidget::SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject)
{
	if (auto slot = SkillSlotMap.Find(skillKey))
		(*slot)->SetSkillSlotVisuals(skillKey, InResoueceObject);
}

void UChampionStatusWidget::SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging)
{
	SkillSlotMap[skillKey]->SetIsCharging(bIsCharging);
}

void UChampionStatusWidget::ChangeChargeCount(ESkillKey skillKey, int32 count)
{
	SkillSlotMap[skillKey]->SetChargeCount(count);
}

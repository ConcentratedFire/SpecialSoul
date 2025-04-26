// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChampionStatusWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
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

void UChampionStatusWidget::SetChargeCount(ESkillKey skillKey, int32 count)
{
	SkillSlotMap[skillKey]->SetChargeCount(count);
}

void UChampionStatusWidget::SetHP(float hp, float maxHP)
{
	HP_Bar->SetPercent(hp/maxHP);

	FString hpText = FString::Printf(TEXT("%.0f / %.0f"), hp, maxHP);
	Text_HP->SetText(FText::FromString(hpText));
}

void UChampionStatusWidget::SetEnergy(float energy, float maxEnergy)
{
	Energy_Bar->SetPercent(energy/maxEnergy);
	
	FString energyText = FString::Printf(TEXT("%.0f / %.0f"), energy, maxEnergy);
	Text_Energy->SetText(FText::FromString(energyText));
}

void UChampionStatusWidget::SetEXP(float exp, float maxExp)
{
	EXP_Bar->SetPercent(exp/maxExp);
}

void UChampionStatusWidget::SetLevel(int32 level)
{
	Text_Level->SetText(FText::AsNumber(level));
}

void UChampionStatusWidget::SetPortrait(UObject* Object)
{
	if (Object)
	{
		Image_Portrait->SetBrushFromTexture(Cast<UTexture2D>(Object));
	}
}


void UChampionStatusWidget::SetPassiveImage(UObject* Object)
{
	if (Object)
	{
		Border_Passive->SetBrushFromTexture(Cast<UTexture2D>(Object));
	}
}

void UChampionStatusWidget::SetPassiveText(int32 count)
{
	if (Text_KillCount) Text_KillCount->SetText(FText::AsNumber(count));
}

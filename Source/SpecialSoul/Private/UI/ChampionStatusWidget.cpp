// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChampionStatusWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"
#include "UI/SkillSlotWidget.h"
#include "UI/UpgradeSlotWidget.h"

UChampionStatusWidget::UChampionStatusWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 생성자에서 아이콘 로드
	ConstructorHelpers::FObjectFinder<UTexture2D> YasuoNormalWeaponIcon(TEXT("/Game/Asset/UI/Resource/YasuoNormalWeapon.YasuoNormalWeapon"));
	ConstructorHelpers::FObjectFinder<UTexture2D> YasuoUpgradeWeaponIcon(TEXT("/Game/Asset/UI/Resource/YasuoUpgradeWeapon.YasuoUpgradeWeapon"));
	ConstructorHelpers::FObjectFinder<UTexture2D> JinxNormalWeaponIcon(TEXT("/Game/Asset/UI/Resource/JinxNormalWeapon.JinxNormalWeapon"));
	ConstructorHelpers::FObjectFinder<UTexture2D> JinxUpgradeWeaponIcon(TEXT("/Game/Asset/UI/Resource/JinxUpgradeWeapon.JinxUpgradeWeapon"));
	ConstructorHelpers::FObjectFinder<UTexture2D> DamageIcon(TEXT("/Game/Asset/UI/Resource/DamageUpgrade.DamageUpgrade"));
	ConstructorHelpers::FObjectFinder<UTexture2D> AbilityHasteIcon(TEXT("/Game/Asset/UI/Resource/CoolTimeUpgrade.CoolTimeUpgrade"));
	ConstructorHelpers::FObjectFinder<UTexture2D> ProjectileIcon(TEXT("/Game/Asset/UI/Resource/ProjectileUpgrade.ProjectileUpgrade"));
	ConstructorHelpers::FObjectFinder<UTexture2D> CritChanceIcon(TEXT("/Game/Asset/UI/Resource/CriticalUpgrade.CriticalUpgrade"));

	if (YasuoNormalWeaponIcon.Succeeded()) UpgradeIcons.Add("YasuoNormalWeapon", YasuoNormalWeaponIcon.Object);
	if (YasuoUpgradeWeaponIcon.Succeeded()) UpgradeIcons.Add("YasuoUpgradeWeapon", YasuoUpgradeWeaponIcon.Object);
	if (JinxNormalWeaponIcon.Succeeded()) UpgradeIcons.Add("JinxNormalWeapon", JinxNormalWeaponIcon.Object);
	if (JinxUpgradeWeaponIcon.Succeeded()) UpgradeIcons.Add("JinxUpgradeWeapon", JinxUpgradeWeaponIcon.Object);
	
	if (DamageIcon.Succeeded()) UpgradeIcons.Add("Damage", DamageIcon.Object);
	if (AbilityHasteIcon.Succeeded()) UpgradeIcons.Add("AbilityHaste", AbilityHasteIcon.Object);
	if (ProjectileIcon.Succeeded()) UpgradeIcons.Add("Projectiles", ProjectileIcon.Object);
	if (CritChanceIcon.Succeeded()) UpgradeIcons.Add("CritChance", CritChanceIcon.Object);
}

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

	UpgradeSlots.Add(WBP_UpgradeSlotWidget_0);
	UpgradeSlots.Add(WBP_UpgradeSlotWidget_1);
	UpgradeSlots.Add(WBP_UpgradeSlotWidget_2);
	UpgradeSlots.Add(WBP_UpgradeSlotWidget_3);
	UpgradeSlots.Add(WBP_UpgradeSlotWidget_4);
	UpgradeSlots.Add(WBP_UpgradeSlotWidget_5);

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

void UChampionStatusWidget::SetUpgradeSlot(FString upgradeName, int32 upgradeCount)
{
	if (upgradeName == "YasuoWeapon" )
	{
		Text_WeaponUpgradeCount->SetText(FText::FromString(FString::FromInt(upgradeCount)));
		if (upgradeCount == 6)
		{
			if (UTexture2D** imgPtr = UpgradeIcons.Find("YasuoUpgradeWeapon"))
			{
				if (*imgPtr)
				{
					Image_Weapon->SetBrushFromTexture(*imgPtr, false);
				}
			}
		}
	}
	else if (upgradeName == "JinxWeapon")
	{
		Text_WeaponUpgradeCount->SetText(FText::FromString(FString::FromInt(upgradeCount)));
		if (upgradeCount == 6)
		{
			if (UTexture2D** imgPtr = UpgradeIcons.Find("JinxUpgradeWeapon"))
			{
				if (*imgPtr)
				{
					Image_Weapon->SetBrushFromTexture(*imgPtr, false);
				}
			}
		}
	}
	else
	{
		if (!UpgradeCountSet.Find(upgradeName))
		{
			UpgradeCountSet.Add(upgradeName);
			
			if (UTexture2D** imgPtr = UpgradeIcons.Find(upgradeName))
			{
				if (*imgPtr)
				{
					if (UpgradeSlots.IsValidIndex(occupiedUpgradeSlotCount))
					{
						UpgradeSlots[occupiedUpgradeSlotCount]->SetUpgradeCount(upgradeCount);
						UpgradeSlots[occupiedUpgradeSlotCount]->SetImageIcon(*imgPtr);
						UpgradeSlotIndex.Add(upgradeName, occupiedUpgradeSlotCount);
						occupiedUpgradeSlotCount++;
					}
					else
					{
						LOG_S(Error, TEXT("UpgradeSlots out of range! occupiedUpgradeSlotCount: %d"), occupiedUpgradeSlotCount);
					}
				}
				else
				{
					LOG_S(Error, TEXT("UpgradeIcons image pointer is null for key: %s"), *upgradeName);
				}
			}
		}
		else
		{
			if (int32* slotIndexPtr = UpgradeSlotIndex.Find(upgradeName))
			{
				if (UpgradeSlots.IsValidIndex(*slotIndexPtr))
				{
					UpgradeSlots[*slotIndexPtr]->SetUpgradeCount(upgradeCount);
				}
				else
				{
					LOG_S(Error, TEXT("Invalid UpgradeSlotIndex for key: %s"), *upgradeName);
				}
			}
			else
			{
				LOG_S(Error, TEXT("UpgradeSlotIndex missing key: %s"), *upgradeName);
			}
		}
	}
}

void UChampionStatusWidget::SetDefaultWeaponUI(ACBasePlayer* player)
{
	if (player->IsA<ACYasuo>())
	{
		UTexture2D* img = *UpgradeIcons.Find("YasuoNormalWeapon");
		Image_Weapon->SetBrushFromTexture(Cast<UTexture2D>(img), false);
		Image_Weapon->SetColorAndOpacity(FLinearColor(1,1,1,1)); 
		Text_WeaponUpgradeCount->SetText(FText::FromString(FString::FromInt(1)));
	}
	else if (player->IsA<AJinx>())
	{
		UTexture2D* img = *UpgradeIcons.Find("JinxNormalWeapon");
		Image_Weapon->SetBrushFromTexture(Cast<UTexture2D>(img), false);
		Image_Weapon->SetColorAndOpacity(FLinearColor(1,1,1,1)); 
		Text_WeaponUpgradeCount->SetText(FText::FromString(FString::FromInt(1)));
	}
}

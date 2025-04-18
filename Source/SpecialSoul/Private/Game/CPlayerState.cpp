// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CPlayerState.h"

#include "SpecialSoul.h"
#include "Game/SpecialSoulGameMode.h"
#include "Player/CYasuo.h"
#include "UI/HUD/GameHUD.h"
#include "Utility/CDataSheetUtility.h"

void ACPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode());
	HUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	Player = Cast<ACBasePlayer>(GetPawn());
}

void ACPlayerState::SetInitialData()
{
	if (Player->IsA(ACYasuo::StaticClass()))
	{
		YasuoAttackDataMap = GM->YasuoAttackDataMap;
		YasuoMoveDataMap = GM->YasuoMoveDataMap;
	}
	else
	{
		JinxAttackDataMap = GM->JinxAttackDataMap;
	}
	Player->PrintAttackDataMap();

	UpgradeDataMap = GM->UpgradeDataMap;
}

float ACPlayerState::CalcDamage(float CurDamage, bool& OutbIsCri)
{
	LOG_S(Warning, TEXT("CalcDamage : %f"), CurDamage);
	OutbIsCri = false;
	return CurDamage;
	
	// if (UpgradeDataMap.Num() == 0) return CurDamage;
	//
	// // 추가해야되는 퍼센트
	// int32 CalcDamagePercent = UpgradeDataMap["Damage"].DefaultValue + UpgradeDataMap["Damage"].AppendValue *
	// 	CurDamageGrade;
	// // ex) 220%
	// // 180 + 180 * 220/100 + 180 * 220%100 / 100
	// float quotient = CalcDamagePercent / 100;
	// float remainder = CalcDamagePercent % 100;
	// float ResultDamage = CurDamage + (CurDamage * quotient) + (CurDamage * remainder / 100);
	//
	// // 치명타 퍼센트
	// int32 CriPercent = UpgradeDataMap["CritChance"].DefaultValue + UpgradeDataMap["CritChance"].AppendValue *
	// 	CurCritChanceGrade;
	// if (FMath::RandRange(1, 100) <= CriPercent)
	// {
	// 	OutbIsCri = true;
	// 	return ResultDamage * 1.75; // 치명타 적용
	// }
	// else
	// {
	// 	return ResultDamage; // 치명타 미적용
	// }
}

float ACPlayerState::CalcAbilityHaste(float CurHaste)
{
	if (UpgradeDataMap.Num() == 0) return CurHaste;

	// 추가해야되는 퍼센트
	// 0 + 5 * 3
	float CalcValue = UpgradeDataMap["AbilityHaste"].DefaultValue + UpgradeDataMap["AbilityHaste"].AppendValue *
		CurAbilityHasteGrade;
	// 퍼센트로 변환
	// 100 * 15 / 115
	int32 CalcPercent = FMath::RoundToInt(100 * CalcValue / (100 + CalcValue));
	float quotient = CalcPercent / 100;
	float remainder = CalcPercent % 100;
	return CurHaste + (CurHaste * quotient) + (CurHaste * remainder / 100);
}

int32 ACPlayerState::CalcProjectile(int32 CurProjectile)
{
	if (UpgradeDataMap.Num() == 0) return CurProjectile;

	int32 CalcValue = UpgradeDataMap["Projectiles"].AppendValue * CurProjectilesGrade;
	return CurProjectile + CalcValue;
}

void ACPlayerState::UpdateGradeInfo()
{
	// 초기 데이터 세팅
	// if (UpgradeDataMap.Num() > 0)
	// {
	// 	CurDamageGrade = 1, CurAbilityHasteGrade = 1, CurProjectilesGrade = 1, CurCritChanceGrade = 1;
	// }
}

TArray<FString> ACPlayerState::ChooseUpgradeCardList()
{
	if (UpgradeData.Num() >= 3) ShuffleArray(UpgradeData);

	TArray<FString> CardList;
	int32 CardCount = 0;
	for (int i = 0; i < UpgradeData.Num(); ++i)
	{
		CardList.Push(UpgradeData[i]);
		if (++CardCount == 3)
			break;
	}

	return CardList;
}

void ACPlayerState::UpgradeStat(const FString statName)
{
	if (statName == "Weapon")
	{
		++CurWeaponGrade;
		Player->UpgradeWeapon(CurWeaponGrade); // 플레이어 공통 함수를 통해서 업그레이드 처리
		if (IsMaxUpgrade_Weapon())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "Damage")
	{
		++CurDamageGrade;
		if (IsMaxUpgrade_Damage())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "AbilityHaste")
	{
		++CurAbilityHasteGrade;
		if (IsMaxUpgrade_AbilityHaste())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "Projectiles")
	{
		++CurProjectilesGrade;
		if (IsMaxUpgrade_Projectile())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "CritChance")
	{
		++CurCritChanceGrade;
		if (IsMaxUpgrade_CritChance())
		{
			RemoveArrayElement(statName);
		}
	}

	Player->EndUpgrade();
}

bool ACPlayerState::IsMaxUpgrade_Weapon()
{
	if (Player->IsA(ACYasuo::StaticClass()))
		return CurWeaponGrade == YasuoAttackDataMap.Num();
	else
		return CurWeaponGrade == JinxAttackDataMap.Num();
}

void ACPlayerState::ShuffleArray(TArray<FString>& Array)
{
	FRandomStream Random(FMath::Rand());

	for (int32 i = Array.Num() - 1; i > 0; i--)
	{
		int32 j = Random.RandRange(0, i);
		Array.Swap(i, j);
	}
}

void ACPlayerState::RemoveArrayElement(const FString Element)
{
	UpgradeData.Remove(Element);
}

void ACPlayerState::AddKillScore()
{
	++KillScore;
	HUD->SetKillScore(KillScore);
}

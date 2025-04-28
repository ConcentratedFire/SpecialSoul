// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CPlayerState.h"

#include "SpecialSoul.h"
#include "Game/CGameInstance.h"
#include "Game/SpecialSoulGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/CPlayerController.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"
#include "UI/HUD/GameHUD.h"
#include "Utility/CDataSheetUtility.h"

void ACPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		GM = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode());

	HUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	Player = Cast<ACBasePlayer>(GetPawn());

	if (GetPlayerController() && GetPlayerController()->IsLocalController())
	{
		auto gi = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
		ServerRPC_SetUserName(gi->MySessionName);
	}
}

void ACPlayerState::SRPC_SetInitialData_Implementation()
{
	// if (!Player)
	// 	Player = Cast<ACBasePlayer>(GetPawn());
	//
	// if (Player->IsA(ACYasuo::StaticClass()))
	// {
	// 	YasuoAttackDataMap = GM->YasuoAttackDataMap;
	// 	YasuoMoveDataMap = GM->YasuoMoveDataMap;
	// }
	// else
	// {
	// 	JinxAttackDataMap = GM->JinxAttackDataMap;
	// }

	YasuoAttackDataMap = GM->YasuoAttackDataMap;
	YasuoMoveDataMap = GM->YasuoMoveDataMap;
	JinxAttackDataMap = GM->JinxAttackDataMap;

	// 초기 데이터 세팅
	auto pc = Cast<ACPlayerController>(GetPlayerController());
	SetPlayerCharacterInfo(pc);

	UpgradeDataMap = GM->UpgradeDataMap;
}

float ACPlayerState::CalcDamage(float CurDamage, bool& OutbIsCri)
{
	// LOG_S(Warning, TEXT("CalcDamage : %f"), CurDamage);

	if (UpgradeDataMap.Num() == 0) return CurDamage;

	// 추가해야되는 퍼센트
	int32 CalcDamagePercent = UpgradeDataMap["Damage"].DefaultValue + UpgradeDataMap["Damage"].AppendValue *
		CurDamageGrade;
	// ex) 220%
	// 180 + 180 * 220/100 + 180 * 220%100 / 100
	float quotient = CalcDamagePercent / 100;
	float remainder = CalcDamagePercent % 100;
	float ResultDamage = CurDamage + (CurDamage * quotient) + (CurDamage * remainder / 100);

	// 치명타 퍼센트
	int32 CriPercent = UpgradeDataMap["CritChance"].DefaultValue + UpgradeDataMap["CritChance"].AppendValue *
		CurCritChanceGrade;
	if (FMath::RandRange(1, 100) <= CriPercent)
	{
		OutbIsCri = true;
		return ResultDamage * 1.75; // 치명타 적용
	}
	else
	{
		return ResultDamage; // 치명타 미적용
	}
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

void ACPlayerState::UpgradeStat(const FString& statName)
{
	if (!IsValid(Player))
	{
		LOG_S(Error, TEXT("Player is INVALID in UpgradeStat!"));
		return;
	}

	LOG_S(Warning, TEXT("UpgradeStat : %s"), *statName);

	bool bNeedHUDUpdate = false;
	int32 NewGrade = 0;

	if (statName == "Weapon")
	{
		++CurWeaponGrade;
		Player->UpgradeWeapon(CurWeaponGrade);
		NewGrade = CurWeaponGrade;
		bNeedHUDUpdate = true;

		if (IsMaxUpgrade_Weapon())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "Damage")
	{
		++CurDamageGrade;
		NewGrade = CurDamageGrade;
		bNeedHUDUpdate = true;

		if (IsMaxUpgrade_Damage())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "AbilityHaste")
	{
		++CurAbilityHasteGrade;
		NewGrade = CurAbilityHasteGrade;
		bNeedHUDUpdate = true;

		if (IsMaxUpgrade_AbilityHaste())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "Projectiles")
	{
		++CurProjectilesGrade;
		NewGrade = CurProjectilesGrade;
		bNeedHUDUpdate = true;

		if (IsMaxUpgrade_Projectile())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "CritChance")
	{
		++CurCritChanceGrade;
		NewGrade = CurCritChanceGrade;
		bNeedHUDUpdate = true;

		if (IsMaxUpgrade_CritChance())
		{
			RemoveArrayElement(statName);
		}
	}

	// 여기까지 서버에서 상태 업데이트 완료
	Player->EndUpgrade();

	// HUD 갱신은 클라이언트 RPC
	if (bNeedHUDUpdate && GetPlayerController())
	{
		ACPlayerController* pc = Cast<ACPlayerController>(GetPlayerController());
		if (pc)
		{
			if (statName == "Weapon")
			{
				if (Player->IsA<ACYasuo>())
					pc->CRPC_UpdateUpgradeSlot("YasuoWeapon", NewGrade);
				else if (Player->IsA<AJinx>())
					pc->CRPC_UpdateUpgradeSlot("JinxWeapon", NewGrade);
			}
			else
				pc->CRPC_UpdateUpgradeSlot(statName, NewGrade);
		}
	}
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

void ACPlayerState::OnRep_KillScore()
{
	if (HUD)
		HUD->SetKillScore(KillScore);
}

void ACPlayerState::AddKillScore() // 서버함수에서 호출됌
{
	++KillScore;
	if (GetOwner() && GetOwner()->HasLocalNetOwner())
		HUD->SetKillScore(KillScore);
}

void ACPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ACPlayerState, KillScore, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACPlayerState, CurWeaponGrade, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACPlayerState, CurDamageGrade, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACPlayerState, CurAbilityHasteGrade, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACPlayerState, CurProjectilesGrade, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACPlayerState, CurCritChanceGrade, COND_OwnerOnly);
}

void ACPlayerState::ServerRPC_SetUserName_Implementation(const FString& name)
{
	SetPlayerName(name);
}

void ACPlayerState::MRPC_SetPlayerReady_Implementation(bool _bIsReady)
{
	SetPlayerReady(_bIsReady);
}

void ACPlayerState::SetPlayerCharacterInfo(class ACPlayerController* PC)
{
	Player = Cast<ACBasePlayer>(PC->GetPawn());
	if (!Player) return;
	
	if (PC && Player->IsA(ACYasuo::StaticClass()))
	{
		if (YasuoAttackDataMap.Num() > 0)
			PC->UpgradeWeapon(1);

		if (YasuoMoveDataMap.Num() > 0)
			PC->UpdateYasuoMoveStat(1);
	}
	else if (PC && Player->IsA(AJinx::StaticClass()))
	{
		if (JinxAttackDataMap.Num() > 0)
		{
			PC->UpgradeWeapon(1);
		}

		if (auto jinx = Cast<AJinx>(Player))
		{
			jinx->SRPC_UseSkill(ESkillKey::Attack);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerController.h"

#include "Game/CGameState.h"
#include "Game/CPlayerState.h"
#include "Player/CBasePlayer.h"
#include "Player/CYasuo.h"

ACPlayerController::ACPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}

void ACPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MyPlayer = Cast<ACBasePlayer>(InPawn);
	MyPlayerState = Cast<ACPlayerState>(PlayerState);

	if (HasAuthority())
		GS = Cast<ACGameState>(GetWorld()->GetGameState());
	
	if (HasAuthority() && !IsLocalController())
		MyPlayerState->SRPC_SetInitialData();
}

void ACPlayerController::UpgradeWeapon(const int32 Level)
{
	if (MyPlayer->IsA(ACYasuo::StaticClass()))
	{
		if (!MyPlayerState->YasuoAttackDataMap.Contains(Level)) return;

		const auto& StatData = MyPlayerState->YasuoAttackDataMap[Level];
		MyPlayer->YasuoStat.ID = StatData.ID;
		MyPlayer->YasuoStat.ProjectileCount = StatData.ProjectileCount;
		MyPlayer->YasuoStat.ProjectileRange = StatData.ProjectileRange;
		MyPlayer->YasuoStat.Damage = StatData.Damage;
		MyPlayer->YasuoStat.UseAOE = StatData.UseAOE;
		MyPlayer->YasuoStat.AOELifeTime = StatData.AOELifeTime;
		MyPlayer->YasuoStat.AOEDamage = StatData.AOEDamage;
		MyPlayer->YasuoStat.AOEDamageCoolTime = StatData.AOEDamageCoolTime;
	}
	else
	{
		if (!MyPlayerState->JinxAttackDataMap.Contains(Level)) return;

		const auto& StatData = MyPlayerState->JinxAttackDataMap[Level];
		MyPlayer->JinxAttackData.ID = StatData.ID;
		MyPlayer->JinxAttackData.ProjectileCount = StatData.ProjectileCount;
		MyPlayer->JinxAttackData.ProjectileRange = StatData.ProjectileRange;
		MyPlayer->JinxAttackData.Damage = StatData.Damage;
		MyPlayer->JinxAttackData.Cooltime = StatData.Cooltime;
		MyPlayer->JinxAttackData.UseAP = StatData.UseAP;
		MyPlayer->JinxAttackData.APDamage = StatData.APDamage;
	}
}

void ACPlayerController::UpdateYasuoMoveStat(int32 Level)
{
	if (MyPlayerState->YasuoMoveDataMap.Num() == 0) return;
	if (Level == GS->MaxLevel) return;

	const auto& StatData = MyPlayerState->YasuoMoveDataMap[Level];
	MyPlayer->YasuoMoveInfo.ID = StatData.ID;
	MyPlayer->YasuoMoveInfo.RangeFrom = StatData.RangeFrom;
	MyPlayer->YasuoMoveInfo.RangeTo = StatData.RangeTo;
	MyPlayer->YasuoMoveInfo.StackDistance = StatData.StackDistance;
}

void ACPlayerController::GetNextLevelYasuoMoveStat()
{
	if (GS->GetCurLevel() > MyPlayer->YasuoMoveInfo.RangeTo)
		UpdateYasuoMoveStat(GS->GetCurLevel() + 1);
}

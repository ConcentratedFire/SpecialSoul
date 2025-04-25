// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerController.h"

#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Game/CGameState.h"
#include "Game/CPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/CBasePlayer.h"
#include "Player/CYasuo.h"
#include "UI/GameWidget.h"
#include "UI/HUD/GameHUD.h"
#include "UI/Standby/CStandbyWidget.h"

ACPlayerController::ACPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ACPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPlayerController, bPlayYasuo);
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());

	if (HasAuthority())
		GS = Cast<ACGameState>(GetWorld()->GetGameState());

	MyPlayerState = Cast<ACPlayerState>(PlayerState);

	if (IsLocalController())
	{
		SelectPlayerWidget = Cast<UCStandbyWidget>(CreateWidget(this, SelectPlayerWidgetFactory));
		SelectPlayerWidget->AddToViewport();
	}
	SetPause(true);
}

void ACPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MyPlayer = Cast<ACBasePlayer>(InPawn);

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

float ACPlayerController::CalcHaste(float CurHaste)
{
	if (!MyPlayerState) return CurHaste;
	return MyPlayerState->CalcAbilityHaste(CurHaste);
}

void ACPlayerController::SRPC_UpgradeStat_Implementation(const FString& statName)
{
	MyPlayerState->UpgradeStat(statName);
}

void ACPlayerController::SRPC_ReadyToPlay_Implementation()
{
	bSelectPlayer = !bSelectPlayer;
	MyPlayerState->MRPC_SetPlayerReady(bSelectPlayer);
	if (bSelectPlayer)
	{
		++GS->ReadyPlayer;
		ServerRequestSpawn();
	}
	else
		--GS->ReadyPlayer;
}

void ACPlayerController::SRPC_SelectPlayer_Implementation(bool _bPlayYasuo)
{
	bPlayYasuo = _bPlayYasuo;
}

void ACPlayerController::ServerRequestSpawn()
{
	TArray<APlayerState*> playerArr = GetWorld()->GetGameState()->PlayerArray;
	if (playerArr.Num() > GS->ReadyPlayer) return;

	// GameMode를 통해 Pawn 스폰
	if (ASpecialSoulGameMode* GM = GetWorld()->GetAuthGameMode<ASpecialSoulGameMode>())
		GM->SpawnPlayerCharacter();

	for (TActorIterator<ACPlayerController> It(GetWorld()); It; ++It)
	{
		(*It)->MRPC_PlayGame();
	}
}

void ACPlayerController::MRPC_PlayGame_Implementation()
{
	if (IsLocalController() && SelectPlayerWidget)
	{
		SelectPlayerWidget->RemoveFromParent();
		if (AGameHUD* hud = Cast<AGameHUD>(GetHUD()))
			hud->ShowWidget();
	}

	SetPause(false);
}

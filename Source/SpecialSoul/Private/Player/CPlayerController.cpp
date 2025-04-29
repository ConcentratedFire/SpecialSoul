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
#include "UI/GameEndingWidget.h"
#include "UI/GameWidget.h"
#include "UI/HUD/GameHUD.h"
#include "UI/Standby/CStandbyWidget.h"
#include "Game/SpecialSoulGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "UI/CFailEndingWidget.h"


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

	if (HasAuthority()/* && !IsLocalController()*/)
		MyPlayerState->SRPC_SetInitialData();

	if (HasAuthority())
		MyPlayerState->SetPlayerCharacterInfo(this);

	// if (IsLocalController())
	// MyPlayer->SetLocalInit(this);
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

void ACPlayerController::CRPC_UpdateLevelUI_Implementation(int32 PlayerLevel)
{
	if (!IsLocalController()) return;

	if (auto gameHUD = Cast<AGameHUD>(GetHUD()))
	{
		gameHUD->SetLevel(PlayerLevel);
	}


	// if (auto overheadUI = Cast<UOverheadStatusWidget>(OverheadUIComp->GetWidget()))
	// {
	// 	overheadUI->SetLevel(PlayerLevel);
	// }

	// if (IsLocallyControlled())
	// {
	// 	if (AGameHUD* hud = Cast<AGameHUD>(GetHUD()))
	// 	{
	// 		hud->SetLevel(PlayerLevel);
	// 		hud->SetEXP(ExpPercentage, 100);
	// 	}
	// }
}

void ACPlayerController::CRPC_UpdateExpUI_Implementation(float Exp, float MaxExp)
{
	if (AGameHUD* hud = Cast<AGameHUD>(GetHUD()))
		hud->SetEXP(Exp, MaxExp);
}

void ACPlayerController::SRPC_UpgradeStat_Implementation(const FString& statName)
{
	MyPlayerState->UpgradeStat(statName);
}

void ACPlayerController::CRPC_UpdateUpgradeSlot_Implementation(const FString& StatName, int32 Grade)
{
	if (AGameHUD* hud = Cast<AGameHUD>(GetHUD()))
		hud->SetUpgradeSlot(StatName, Grade);
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

void ACPlayerController::SRPC_SelectPlayer_Implementation(const bool bInPlayYasuo)
{
	bPlayYasuo = bInPlayYasuo;
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

void ACPlayerController::UpdateStatUI()
{
}

void ACPlayerController::ShowBossUI(AMainBoss* mainBoss, bool bShow)
{
	if (AGameHUD* hud = Cast<AGameHUD>(GetHUD()))
	{
		hud->ShowBossUI(mainBoss, bShow);
	}
}

void ACPlayerController::SetBossHPPercent(float percent)
{
	if (AGameHUD* hud = Cast<AGameHUD>(GetHUD()))
	{
		hud->SetBossHPBarPercent(percent);
	}
}

// void ACPlayerController::ShowGameEndingUI(bool bWin)
// {
// 	if (IsLocalController())
// 	{
// 		if (bWin)
// 		{
// 			auto winUI = CreateWidget<UGameEndingWidget>(this, WinWidgetClass);
// 			winUI->AddToViewport();
// 		}
// 		else
// 		{
// 			auto defeatUI = CreateWidget<UGameEndingWidget>(this, DefeatWidgetClass);
// 			defeatUI->AddToViewport();
// 		}
// 	}
// }

void ACPlayerController::CRPC_ShowGameEndingUI_Implementation(bool bWin)
{
	if (!IsLocalController()) return;

	auto widgetClass = bWin ? WinWidgetClass : DefeatWidgetClass;
	if (widgetClass)
	{
		if (bWin)
		{
			UGameEndingWidget* ui = CreateWidget<UGameEndingWidget>(this, widgetClass);
			if (ui)
				ui->AddToViewport();
		}
		else
		{
			UCFailEndingWidget* ui = CreateWidget<UCFailEndingWidget>(this, widgetClass);
			if (ui)
				ui->AddToViewport();
		}
	}
}

void ACPlayerController::ClientTravelToBattleMap()
{
	FString url = TEXT("/Game/Level/BattleMap.BattleMap");
	ClientTravel(url, TRAVEL_Absolute);
}

void ACPlayerController::ServerTravelToBattleMap()
{
	FString url = TEXT("/Game/Level/BattleMap.BattleMap");
	GetWorld()->ServerTravel(url, TRAVEL_Absolute);
}

void ACPlayerController::SRPC_EndDieProcess_Implementation()
{
	// 관전자가 플레이어의 위치에 생성될 수 있도록 플레이어 정보를 가져온다.
	auto player = Cast<ACBasePlayer>(GetPawn());

	if (player)
	{
		auto gm = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode());
		if (gm && gm->SpectatorClass)
		{
			// 관전자 생성
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APawn* spectator = GetWorld()->SpawnActor<APawn>(gm->SpectatorClass, player->GetCamTransform(), params);

			// 빙의(Possess)
			Possess(spectator);

			// 이전 플레이어 제거
			player->Destroy();
		}
	}
}

void ACPlayerController::SRPC_AddDeadPlayer_Implementation()
{
	++GS->DeadPlayer;
	ServerRequestPlayEnd();
}

void ACPlayerController::ServerRequestPlayEnd()
{
	TArray<APlayerState*> playerArr = GetWorld()->GetGameState()->PlayerArray;
	if (playerArr.Num() > GS->DeadPlayer) return;

	GS->PlayFail();
}

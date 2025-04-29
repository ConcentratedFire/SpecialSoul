// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

class UGameEndingWidget;
class UCFailEndingWidget;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UCStandbyWidget> SelectPlayerWidgetFactory;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UCSelectUpgradeWidget> SelectUpgradeUIWidgetFactory;

	UPROPERTY()
	class UCStandbyWidget* SelectPlayerWidget;
	UPROPERTY()
	class UCSelectUpgradeWidget* SelectUpgradeWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UGameEndingWidget> WinWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UCFailEndingWidget> DefeatWidgetClass;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void CRPC_UpdateUpgradeSlot(const FString& StatName, int32 Grade);
	
	void ShowBossUI(AMainBoss* mainBoss, bool bShow);
	void SetBossHPPercent(float percent);
	//void ShowGameEndingUI(bool bWin);
	void ClientTravelToBattleMap();
	void ServerTravelToBattleMap();

	UFUNCTION(Client, Reliable)
	void CRPC_ShowGameEndingUI(bool bWin);

private:
	ACPlayerController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY()
	class ACGameState* GS;
	UPROPERTY()
	class ACPlayerState* MyPlayerState;
	UPROPERTY()
	class ACBasePlayer* MyPlayer;

public:
	void UpgradeWeapon(int32 Level);
	void UpdateYasuoMoveStat(int32 Level);

public: // For Yasuo
	void GetNextLevelYasuoMoveStat();

public:
	float CalcHaste(float CurHaste);

public:
	UFUNCTION(Server, Reliable)
	void SRPC_UpgradeStat(const FString& statName);
	
	UFUNCTION(Client, Reliable)
	void CRPC_UpdateExpUI(float Exp, float MaxExp);

	UFUNCTION(Client, Reliable)
	void CRPC_UpdateLevelUI(int32 PlayerLevel);
	
public:
	UPROPERTY(Replicated)
	bool bPlayYasuo = true;
	bool bSelectPlayer = false;

	UFUNCTION(Server, Reliable)
	void SRPC_SelectPlayer(const bool bInPlayYasuo);
	
	UFUNCTION(Server, Reliable)
	void SRPC_ReadyToPlay();
	void ServerRequestSpawn();

	UFUNCTION(NetMulticast, Reliable)
	void MRPC_PlayGame();

public:
	void UpdateStatUI();

public:
	UFUNCTION(Server, Reliable)
	void SRPC_EndDieProcess();

public:
	UFUNCTION(Server, Reliable)
	void SRPC_AddDeadPlayer();
	void ServerRequestPlayEnd();
};

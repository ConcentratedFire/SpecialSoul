// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void CRPC_UpdateUpgradeSlot(const FString& StatName, int32 Grade);

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
};

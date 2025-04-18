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
	TSubclassOf<class UCSelectUpgradeWidget> SelectUpgradeUIWidgetFactory;

	UPROPERTY()
	class UCSelectUpgradeWidget* SelectUpgradeWidget;

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
};

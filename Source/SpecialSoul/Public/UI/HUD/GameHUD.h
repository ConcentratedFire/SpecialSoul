// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UGameWidget;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetKillScore(int32 KillScore);
	
public:
	//AGameHUD();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameWidget> GameWidgetClass;

	TObjectPtr<UGameWidget> GameWidget;
	// TObjectPtr<UGameWidget> GameWidget;
};

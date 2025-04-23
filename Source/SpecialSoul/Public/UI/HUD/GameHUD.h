// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Player/Components/SkillComponent.h"
#include "GameHUD.generated.h"

enum class ESkillKey : uint8;
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
	void SetTime(float Time);

	void UpdateSkillCooltime(ESkillKey skillKey, FSkillCooltime cooltimeInfo);
	void SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap);

public:
	//AGameHUD();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameWidget> GameWidgetClass;

	UPROPERTY()
	TObjectPtr<UGameWidget> GameWidget;
};

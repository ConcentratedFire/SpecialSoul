// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Player/CBasePlayer.h"
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
	void SetSkillSlotIsCharging(ESkillKey skillKey, bool bIsCharging);

public:
	//AGameHUD();

	virtual void BeginPlay() override;
	void SetHP(float HP, float MaxHP);
	void SetSkillSlotVisuals(ESkillKey SkillKey, UObject* Object);
	void SetPortrait(UObject* Object);
	void SetPassiveImage(UObject* Object);
	void SetPassiveText(int32 count);
	void SetLevel(int32 level);
	void SetEnergy(float energy, float maxEnergy);
	void SetEXP(float exp, float maxExp);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameWidget> GameWidgetClass;

	UPROPERTY()
	TObjectPtr<UGameWidget> GameWidget;

	void ShowWidget();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CBasePlayer.h"
#include "Jinx.generated.h"

class ISkillStrategy;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API AJinx : public ACBasePlayer
{
	GENERATED_BODY()

public:
	AJinx();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION()
	void CastSkill()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TScriptInterface<ISkillStrategy> ESkill;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TScriptInterface<ISkillStrategy> RSkill;
	
};

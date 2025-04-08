// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Player/CBasePlayer.h"
#include "Jinx.generated.h"

class UJinxAnim;
class ISkillStrategy;

UENUM(BlueprintType)
enum class ESkillKey : uint8
{
	Attack, Passive, E, R
};


UCLASS()
class SPECIALSOUL_API AJinx : public ACBasePlayer
{
	GENERATED_BODY()

public:
	AJinx();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jinx")
	TObjectPtr<UJinxAnim> Anim;

	void CastSkill(ESkillKey Key);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, TScriptInterface<ISkillStrategy>> SkillMap;
	
	
};

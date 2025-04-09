// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "UObject/NoExportTypes.h"
#include "Jinx_Attack.generated.h"

class AMinigunBullet;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UJinx_Attack : public UObject, public ISkillStrategy
{
	GENERATED_BODY()

public:
	virtual void UseSkill(AActor* Caster) override;
	
	UPROPERTY(EditAnywhere, Category = "Jinx")
	TSubclassOf<AMinigunBullet> MinigunBullet;
};

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
	UJinx_Attack();
	
	virtual void UseSkill(ACBasePlayer* Caster) override;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AMinigunBullet> BulletClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	int32 BulletNum = 5;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float FireDelay = 0.05;
	
private:
	FTimerHandle FireTimer;
	int32 FiredBulletNum {0};

};

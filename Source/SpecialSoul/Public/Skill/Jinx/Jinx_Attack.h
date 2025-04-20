// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "UObject/NoExportTypes.h"
#include "Jinx_Attack.generated.h"

class AJinx;
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
	
	virtual void UseSkill(ACharacter* Caster) override;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AMinigunBullet> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	int32 TotalShot = 3;

	UPROPERTY(EditAnywhere, Category = "Attack")
	int32 OneShotBullet = 3; // 한번에 3개씩 쏘기

	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShotDelay = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float OneShotDelay = 0.02f;
	
private:
	void HandleShot(ACharacter* Caster, AJinx* Jinx);
	void HandleOneShot(ACharacter* Caster, FRotator SpawnRot);
	
	int32 ShotCount {0};		// 전체 발사 수
	int32 ShotBulletCount {0};	// 한 발사당 쏘는 총알 수 
	
	FTimerHandle ShotTimer; 
	FTimerHandle OneShotTimer; 
};

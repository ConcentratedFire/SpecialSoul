// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBaseItem.h"
#include "Player/CBasePlayer.h"
#include "CExpMagnet.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACExpMagnet : public ACBaseItem
{
	GENERATED_BODY()

private:
	ACExpMagnet();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CBaseItem.h"
#include "CExp.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACExp : public ACBaseItem
{
	GENERATED_BODY()

private:
	ACExp();

	UFUNCTION()
	void OnExpBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

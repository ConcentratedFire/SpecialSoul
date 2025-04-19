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
	virtual void ActiveItem() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	void MagnetDrop(class ACBasePlayer* Player);

private:
	UPROPERTY(EditDefaultsOnly, Category=Comp)
	class UBoxComponent* DropRangeComp;

	UFUNCTION()
	void OnItemOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Exp")
	int32 ExpCount = 2;

private:
	float RunningTime = 0.0f;

	UPROPERTY()
	class ACBasePlayer* DropPlayer = nullptr;

	bool bCanDrop = false;

private:
	UFUNCTION(Server, Reliable)
	void SRPC_DropItem();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWindWall.generated.h"

UCLASS()
class SPECIALSOUL_API ACWindWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACWindWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }

private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY()
	class ACYasuo* OwnerYasuo;
	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;

	FVector StartLocation;
	FVector TargetLocation;
	float Alpha;

	FVector StartScale=FVector(0);
	FVector TargetScale=FVector(1);

	FTimerHandle PoolTimer;
	float PoolTime = 3.f;

private:
	UFUNCTION(Server, Unreliable)
	void SRPC_MoveWall();
	UFUNCTION(NetMulticast, Unreliable)
	void MRPC_WallScale(const FVector& NewScale);
};

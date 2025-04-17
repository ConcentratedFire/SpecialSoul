// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMiddleBossBullet.generated.h"

UCLASS()
class SPECIALSOUL_API ACMiddleBossBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACMiddleBossBullet();

public:
	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	
	UPROPERTY(EditAnywhere)
	FVector StartLocation;

	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	float MoveDuration = 2.0f;

private:
	float CurrentTime = 0.0f;
	float easeInOutExpo(float x);
};

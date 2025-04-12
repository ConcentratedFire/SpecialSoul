// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBaseItem.generated.h"

UCLASS()
class SPECIALSOUL_API ACBaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACBaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTornado.generated.h"

UCLASS()
class SPECIALSOUL_API ACTornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTornado();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FVector TornadoDirection;

	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;
	
public:
	void SetManager(class ACObjectPoolManager* Manager){ObjectPoolManager = Manager;}
	
private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* TornadoBox;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* TornadoMesh1;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* TornadoMesh2;
};

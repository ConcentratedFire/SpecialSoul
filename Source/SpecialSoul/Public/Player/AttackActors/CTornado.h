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
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

public:
	FVector TornadoStartLocation;

	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;

public:
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }
	
private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* TornadoBox;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* TornadoMesh1;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* TornadoMesh2;

	UPROPERTY(EditDefaultsOnly, category = Attack)
	float Range = 850;
	UPROPERTY(EditDefaultsOnly, category = Attack)
	float Damage = 100;
	UPROPERTY(EditDefaultsOnly, category = Attack)
	float Speed = 800;

	FVector StartLocation;
	bool IsCri;

private:
	UPROPERTY()
	class ACYasuo* OwnerYasuo;

	UFUNCTION()
	void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);

private:
	UFUNCTION(server, Unreliable)
	void SRPC_MoveTornado(const float DeltaTime);

public:
	virtual void SetOwner(AActor* NewOwner) override;
};

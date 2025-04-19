// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTornado_E.generated.h"

UCLASS()
class SPECIALSOUL_API ACTornado_E : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACTornado_E();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

public:
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }

private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* TornadoBox;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* TornadoMesh1;

	UFUNCTION()
	void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);

private:
	UPROPERTY()
	class ACYasuo* OwnerYasuo;
	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;

	UPROPERTY(EditDefaultsOnly, category = Attack, Replicated)
	float Damage = 100;
	bool IsCri;

	UPROPERTY(EditDefaultsOnly, category = Attack)
	FRotator Rot{0, 5, 0};

	float AppendYaw =0;

private:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(server, Unreliable)
	void SRPC_MoveTornado();

public:
	virtual void SetOwner(AActor* NewOwner) override;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CBaseItem.h"

#include "Components/BoxComponent.h"

// Sets default values
ACBaseItem::ACBaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetCollisionProfileName(FName("Item"));
	RootComponent = BoxComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACBaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


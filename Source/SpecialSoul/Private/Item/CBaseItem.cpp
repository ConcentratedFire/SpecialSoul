// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CBaseItem.h"

#include "Components/BoxComponent.h"
#include "Game/CGameState.h"
#include "Game/SpecialSoulGameMode.h"

// Sets default values
ACBaseItem::ACBaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetCollisionProfileName(FName("Item"));
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	RootComponent = BoxComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACBaseItem::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GS = GM->GetGameState<ACGameState>();
	}
}

// Called every frame
void ACBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACBaseItem::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (!bNewHidden)
	{
		FVector UpLocation = GetActorLocation();
		UpLocation.Z += BoxComp->GetScaledBoxExtent().Z;
		SetActorLocation(UpLocation);	
	}	
}
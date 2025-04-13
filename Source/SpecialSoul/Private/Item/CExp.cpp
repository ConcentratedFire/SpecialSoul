// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CExp.h"

#include "Components/BoxComponent.h"
#include "Game/CGameState.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CBasePlayer.h"

ACExp::ACExp()
{
	SetActorScale3D(FVector(0.4f));

	DropRangeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("DropRangeComp"));
	DropRangeComp->SetupAttachment(RootComponent);
	DropRangeComp->SetBoxExtent(FVector(300, 300, 10));
	DropRangeComp->SetCollisionProfileName(FName("Item"));
	DropRangeComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	DropRangeComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	
	DropRangeComp->OnComponentBeginOverlap.AddDynamic(this, &ACExp::OnItemOverlapBegin);
}

void ACExp::ActiveItem()
{
	if (!GS) return;
	GS->AddExp(ExpCount);

	if (ObjectPoolManager)
		ObjectPoolManager->ReturnExp(this);
}

void ACExp::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bCanDrop || !DropPlayer) return;

	FVector Pos = GetActorLocation();
	Pos = FMath::Lerp(Pos, DropPlayer->GetActorLocation(), 10 * GetWorld()->DeltaRealTimeSeconds);

	SetActorLocation(Pos, true);

	float Dist = FVector::Distance(Pos, DropPlayer->GetActorLocation());
	if (Dist < 50)
	{
		SetActorLocation(DropPlayer->GetActorLocation(), true);
		ActiveItem();
	}
}

void ACExp::OnItemOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
	if (DropPlayer) return;

	DropPlayer = Cast<ACBasePlayer>(OtherActor);
	bCanDrop = true;
}

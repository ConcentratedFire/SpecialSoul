// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CExp.h"

#include "Components/BoxComponent.h"

ACExp::ACExp()
{
	SetActorScale3D(FVector(0.4f));

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACExp::OnExpBeginOverlap);
}

void ACExp::OnExpBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

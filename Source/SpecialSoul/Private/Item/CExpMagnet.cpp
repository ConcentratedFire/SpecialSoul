// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CExpMagnet.h"

#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "Item/CExp.h"
#include "ObjectPool/CObjectPoolManager.h"

ACExpMagnet::ACExpMagnet()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACExpMagnet::OnOverlapBegin);
}

void ACExpMagnet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Player = Cast<ACBasePlayer>(OtherActor))
	{
		for (TActorIterator<ACExp> It(GetWorld(), ACExp::StaticClass()); It; ++It)
		{
			ACExp* exp = *It;
			if (!exp->IsHidden())	(*It)->MagnetDrop(Player);			
		}

		ObjectPoolManager->ReturnExpMagnet(this);
	}
}
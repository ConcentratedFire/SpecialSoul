// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CItemBox.h"

#include "Components/BoxComponent.h"
#include "ObjectPool/CObjectPoolManager.h"

ACItemBox::ACItemBox()
{
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
}

void ACItemBox::ActiveItem()
{
	SelectItem();
	ObjectPoolManager->ReturnItemBox(this);
}

void ACItemBox::SelectItem()
{
	FTransform Transform(FRotator::ZeroRotator, GetActorLocation(), FVector(1));
	ObjectPoolManager->MagnetSpawn(Transform);
	// 20% 확률로 꽝
	// 40% 확률로 힐
	// 30% 확률로 폭탄
	// 10% 확률로 무지개구슬
	// float rand = FMath::FRandRange(1.0f, 100.0f);
	// if (rand > 80.f)
	// {
	// 	// 꽝
	// 	return;
	// }
	// else if (rand > 40.f)
	// {
	// 	// 힐
	// }
	// else if (rand > 10.f)
	// {
	// 	// 폭탄
	// }
	// else
	// {
	// 	// 무지개 구슬
	// }
}

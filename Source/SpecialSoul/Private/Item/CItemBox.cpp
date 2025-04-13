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
	ObjectPoolManager->ReturnItemBox(this);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CExp.h"

#include "Game/CGameState.h"

ACExp::ACExp()
{
	SetActorScale3D(FVector(0.4f));
}

void ACExp::ActiveItem()
{
	if (!GS) return;
	GS->AddExp(ExpCount);	
}
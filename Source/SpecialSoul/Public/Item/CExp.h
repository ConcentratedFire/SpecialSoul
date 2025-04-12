// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CBaseItem.h"
#include "CExp.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACExp : public ACBaseItem
{
	GENERATED_BODY()

private:
	ACExp();
	virtual void ActiveItem() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Exp")
	int32 ExpCount = 2;
};

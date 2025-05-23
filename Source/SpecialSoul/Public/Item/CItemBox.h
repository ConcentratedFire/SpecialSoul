// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CBaseItem.h"
#include "CItemBox.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACItemBox : public ACBaseItem
{
	GENERATED_BODY()

private:
	ACItemBox();
	virtual void ActiveItem() override;
	void SelectItem();

	UFUNCTION(Server, Reliable)
	void SRPC_PlaceItem();
};

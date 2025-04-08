// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Jinx_Attack.generated.h"

class AMinigunBullet;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UJinx_Attack : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "Jinx")
	TSubclassOf<AMinigunBullet> MinigunBullet;
	
	
};

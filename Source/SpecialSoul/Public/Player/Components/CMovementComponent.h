// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPECIALSOUL_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCMovementComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class ACBasePlayer* BaseOwnerCharacter;
	UPROPERTY()
	class ACPlayerController* PC;

private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* IA_Move;

	UFUNCTION()
	void SetInputBinding(class UEnhancedInputComponent* Input);
	UFUNCTION()
	void Move(const struct FInputActionValue& Value);

private:
	UPROPERTY(EditDefaultsOnly, Category = Rotation)
	float RoationInteropSpeed = 5.f;
	
	void RotationToMouseCursor(const float& DeltaTime);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBasePlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*)

UCLASS()
class SPECIALSOUL_API ACBasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACBasePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public: // Input
	FInputBindingDelegate OnInputBindingDel;

public: // Return Component
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void Attack() PURE_VIRTUAL(); // Attack 기본 함수

private: // Input
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputMappingContext* IMC_Player;

private: // Component
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

private: // Actor Component
	UPROPERTY(EditDefaultsOnly)
	class UCMovementComponent* MoveComp;
};

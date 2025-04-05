// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "Player/CBasePlayer.h"

// Sets default values for this component's properties
UCMovementComponent::UCMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	ConstructorHelpers::FObjectFinder<UInputAction> tempIA(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_Move.IA_Move'"));
	if (tempIA.Succeeded())
		IA_Move = tempIA.Object;
}

void UCMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner())
	{
		BaseOwner = Cast<ACBasePlayer>(GetOwner());
		if (BaseOwner)
		{
			BaseOwner->OnInputBindingDel.AddUObject(this, &UCMovementComponent::SetInputBinding);
		}
	}
}


// Called when the game starts
void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCMovementComponent::SetInputBinding(class UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UCMovementComponent::Move);
}

void UCMovementComponent::Move(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();

	FVector direction(v.X, v.Y, 0);
	BaseOwner->AddMovementInput(BaseOwner->GetTransform().TransformVector(direction));
}

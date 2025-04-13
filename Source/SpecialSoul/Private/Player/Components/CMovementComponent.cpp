// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "Player/CBasePlayer.h"
#include "Player/CPlayerController.h"
#include "Player/CYasuo.h"

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
		BaseOwnerCharacter = Cast<ACBasePlayer>(GetOwner());
		if (BaseOwnerCharacter)
		{
			BaseOwnerCharacter->OnInputBindingDel.AddUObject(this, &UCMovementComponent::SetInputBinding);
		}
	}
}


// Called when the game starts
void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!BaseOwnerCharacter || !BaseOwnerCharacter->IsLocallyControlled()) return;
	
	PC = Cast<ACPlayerController>(BaseOwnerCharacter->GetController());

	YasuoCharacer = Cast<ACYasuo>(PC->GetPawn());
	if (YasuoCharacer)
		BeforeLocation = YasuoCharacer->GetActorLocation();
}


// Called every frame
void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotationToMouseCursor(DeltaTime);

	// 플레이중인 캐릭터가 야스오 일때, 이동거리를 체크하고 기력을 충전시킴
	if (YasuoCharacer)
	{
		FVector CurrentLocation = YasuoCharacer->GetActorLocation();
		float Distance = FVector::Dist(CurrentLocation, BeforeLocation);
		YasuoCharacer->MoveDistance += Distance;
		BeforeLocation = CurrentLocation;
	}
}

void UCMovementComponent::SetInputBinding(class UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UCMovementComponent::Move);
}

void UCMovementComponent::Move(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();

	FVector direction(v.X, v.Y, 0);
	// BaseOwnerCharacter->AddMovementInput(BaseOwnerCharacter->GetTransform().TransformVector(direction));
	BaseOwnerCharacter->AddMovementInput(direction);
}

void UCMovementComponent::RotationToMouseCursor(const float& DeltaTime)
{
	if (!PC) return;

	FHitResult HitResult;
	bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (bHit)
	{
		FVector directionToMouseCursor = HitResult.Location - BaseOwnerCharacter->GetActorLocation();
		directionToMouseCursor.Z = 0;
		directionToMouseCursor.Normalize();

		// 목표 회전값
		FRotator targetRot = FRotationMatrix::MakeFromX(directionToMouseCursor).Rotator();

		// 현재 회전값에서 목표 회전값으로 보간
		FRotator CurrentRotation = BaseOwnerCharacter->GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, targetRot, DeltaTime, RoationInteropSpeed);

		// 회전 적용
		BaseOwnerCharacter->SetActorRotation(NewRotation);
	}
}

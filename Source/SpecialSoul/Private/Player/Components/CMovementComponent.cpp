// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "SpecialSoul.h"
#include "Net/UnrealNetwork.h"
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

	// LOG_S(Warning, TEXT("GetOwner : %s"), GetOwner()?TEXT("true"):TEXT("false"));
	if (GetOwner())
	{
		BaseOwnerCharacter = Cast<ACBasePlayer>(GetOwner());
		// LOG_S(Warning, TEXT("BaseOwnerCharacter : %s"), BaseOwnerCharacter?TEXT("true"):TEXT("false"));
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
	//
	// if (!BaseOwnerCharacter || !BaseOwnerCharacter->IsLocallyControlled()) return;
	//
	// PC = Cast<ACPlayerController>(BaseOwnerCharacter->GetController());
}


// Called every frame
void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CRPC_RotationToMouseCursor();

	// 플레이중인 캐릭터가 야스오 일때, 이동거리를 체크하고 기력을 충전시킴
	if (YasuoCharacer && YasuoCharacer->IsLocallyControlled())
	{
		CRPC_CheckMoveDistance();
	}
}

void UCMovementComponent::SetInputBinding(class UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UCMovementComponent::Move);
}

void UCMovementComponent::Move(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	FVector2D v = Value.Get<FVector2D>();

	FVector direction(v.X, v.Y, 0);
	// BaseOwnerCharacter->AddMovementInput(BaseOwnerCharacter->GetTransform().TransformVector(direction));
	BaseOwnerCharacter->AddMovementInput(direction);
}

void UCMovementComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCMovementComponent, BeforeLocation);
}

void UCMovementComponent::SetController(class ACPlayerController* NewController)
{
	PC = NewController;

	YasuoCharacer = Cast<ACYasuo>(PC->GetPawn());
	if (YasuoCharacer)
	{
		BeforeLocation = YasuoCharacer->GetActorLocation();
	}
}

void UCMovementComponent::CRPC_RotationToMouseCursor_Implementation()
{
	if (!PC) return;
	// if (!bCanMove) return;

	FHitResult HitResult;
	bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (bHit)
	{
		FVector directionToMouseCursor = HitResult.Location - BaseOwnerCharacter->GetActorLocation();
		directionToMouseCursor.Z = 0;
		directionToMouseCursor.Normalize();

		// 회전 적용
		SRPC_RotationToMouseCursor(directionToMouseCursor);
	}
}

void UCMovementComponent::SRPC_RotationToMouseCursor_Implementation(const FVector MouseDirection)
{
	// 목표 회전값
	FRotator targetRot = FRotationMatrix::MakeFromX(MouseDirection).Rotator();

	// 현재 회전값에서 목표 회전값으로 보간
	FRotator CurrentRotation = BaseOwnerCharacter->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, targetRot,
	                                        BaseOwnerCharacter->GetWorld()->GetDeltaSeconds(), RoationInteropSpeed);
	MRPC_RotationToMouseCursor(NewRotation);
}

void UCMovementComponent::MRPC_RotationToMouseCursor_Implementation(const FRotator NewRotation)
{
	BaseOwnerCharacter->SetActorRotation(NewRotation);
}

void UCMovementComponent::CRPC_CheckMoveDistance_Implementation()
{
	FVector CurrentLocation = YasuoCharacer->GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation, BeforeLocation);
	BeforeLocation = CurrentLocation;
	if (!YasuoCharacer) return;
	YasuoCharacer->MoveDistance += Distance;
}

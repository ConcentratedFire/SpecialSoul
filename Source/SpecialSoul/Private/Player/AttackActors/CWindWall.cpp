// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AttackActors/CWindWall.h"

#include "Components/BoxComponent.h"
#include "ObjectPool/CObjectPoolManager.h"

// Sets default values
ACWindWall::ACWindWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(10, 80, 60));
	BoxComp->SetCollisionProfileName(FName("WindWall"));
	RootComponent = BoxComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeLocationAndRotation(FVector(10, 0, -60), FRotator(0, -90, 0));
	MeshComp->SetRelativeScale3D(FVector(.3, .1, .2));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACWindWall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACWindWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Alpha < 1.f)
	{
		Alpha += DeltaTime;
		if (Alpha >= 1.f)
		{
			SetActorLocation(TargetLocation);
			SetActorScale3D(TargetScale);
		}
		else
		{
			SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, Alpha));
			SetActorScale3D(FMath::Lerp(StartScale, TargetScale, Alpha));
		}
	}
	else
	{
		if (!GetWorldTimerManager().IsTimerActive(PoolTimer))
		{
			// 설치 완료 후 3초 이후 사라짐
			GetWorldTimerManager().SetTimer(PoolTimer, [this]
			{
				ObjectPoolManager->ReturnWindWall(this);
			}, PoolTime, false);
		}
	}
}

void ACWindWall::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (!bNewHidden)
	{
		FVector UpLocation = GetActorLocation();
		UpLocation.Z += BoxComp->GetScaledBoxExtent().Z;
		SetActorLocation(UpLocation);

		Alpha = 0.f;
		StartLocation = GetActorLocation();
		TargetLocation = GetActorLocation() + GetActorForwardVector() * 150;

		SetActorScale3D(StartScale);

		GetWorldTimerManager().ClearTimer(PoolTimer);
	}
}

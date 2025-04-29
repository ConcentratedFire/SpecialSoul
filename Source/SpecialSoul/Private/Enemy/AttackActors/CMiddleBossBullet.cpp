// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AttackActors/CMiddleBossBullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "ObjectPool/CObjectPoolManager.h"

// Sets default values
ACMiddleBossBullet::ACMiddleBossBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACMiddleBossBullet::OnMyOverlap);
	SphereComp->SetSphereRadius(55.f);
	SphereComp->SetCollisionProfileName(FName("Enemy"));

	SphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Overlap);
	RootComponent = SphereComp;

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ACMiddleBossBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACMiddleBossBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!OwnerMiddleBoss) return;
	if (!OwnerMiddleBoss->HasAuthority()) return;
	SRPC_MoveBullet();
}

void ACMiddleBossBullet::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (HasAuthority() && !bNewHidden)
	{
		StartLocation = GetActorLocation();
		TargetLocation = GetActorLocation() + GetActorForwardVector() * 500;
		CurrentTime = 0.f;
		bReturnPool = false;
	}
}

void ACMiddleBossBullet::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	OwnerMiddleBoss = Cast<ACMiddleBoss>(NewOwner);
}

float ACMiddleBossBullet::easeInOutExpo(float x)
{
	return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? FMath::Pow(2, 20 * x - 10) / 2 : (2 - FMath::Pow(2, -20 * x + 10)) / 2;
}

void ACMiddleBossBullet::SRPC_MoveBullet_Implementation()
{
	if (CurrentTime <= MoveDuration)
	{
		float Alpha = CurrentTime / MoveDuration; // 0에서 1로 진행
		float EasedAlpha = easeInOutExpo(Alpha);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, EasedAlpha);
		SetActorLocation(NewLocation);
		CurrentTime += GetWorld()->GetDeltaSeconds();
	}
	else
	{
		if (bReturnPool) return;
		ObjectPoolManager->ReturnMiddleBossBullet(this);
	}
}

void ACMiddleBossBullet::OnMyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (auto player = Cast<ACBasePlayer>(OtherActor))
	{
		//LOG_S(Warning, TEXT("Middle Boss Bullet Damage"));
		player->DamageProcess(Damage);
	}
	else if (OtherActor->IsA(ACWindWall::StaticClass()))
	{
		//LOG_S(Warning, TEXT("Middle Boss Bullet Return"));
		ObjectPoolManager->ReturnMiddleBossBullet(this);
		bReturnPool = true;
	}
}

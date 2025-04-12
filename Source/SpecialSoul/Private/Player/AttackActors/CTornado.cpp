// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AttackActors/CTornado.h"

#include "SpecialSoul.h"
#include "Components/BoxComponent.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CYasuo.h"

// Sets default values
ACTornado::ACTornado()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TornadoBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TornadoBox"));
	RootComponent = TornadoBox;

	TornadoMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TornadoMesh1"));
	TornadoMesh1->SetupAttachment(TornadoBox);
	TornadoMesh1->SetRelativeScale3D(FVector(.3f));
	TornadoMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TornadoMesh2"));
	TornadoMesh2->SetupAttachment(TornadoBox);
	TornadoMesh2->SetRelativeScale3D(FVector(.45f, .45f, .3f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tmpTornado(
		TEXT("/Script/Engine.StaticMesh'/Game/Asset/Tornado/Mesh/SM_Circle_size04.SM_Circle_size04'"));
	if (tmpTornado.Succeeded())
	{
		TornadoMesh1->SetStaticMesh(tmpTornado.Object);
		TornadoMesh2->SetStaticMesh(tmpTornado.Object);
	}

	TornadoBox->SetCollisionProfileName(FName("PlayerAttack"));
	TornadoMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TornadoMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TornadoBox->OnComponentBeginOverlap.AddDynamic(this, &ACTornado::OnCompBeginOverlap);
}

// Called when the game starts or when spawned
void ACTornado::BeginPlay()
{
	Super::BeginPlay();

	OwnerYasuo = Cast<ACYasuo>(GetOwner());
	LOG_S(Warning, TEXT("OwnerYasuo : %s"), OwnerYasuo==nullptr?TEXT("Owner Null"):*OwnerYasuo->GetName());
	// 이동 경로 방향으로 회전
	// if (!TornadoDirection.IsZero())
	// {
	// 	SetActorRotation((TornadoDirection).Rotation());
	// }
}

void ACTornado::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (!bNewHidden)
	{
		FVector UpLocation = GetActorLocation();
		UpLocation.Z += TornadoBox->GetScaledBoxExtent().Z;
		SetActorLocation(UpLocation);
		
		TornadoStartLocation = GetActorLocation();		
	}
}

// Called every frame
void ACTornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);

	FRotator meshRot1 = TornadoMesh1->GetRelativeRotation();
	meshRot1.Yaw += 10;
	TornadoMesh1->SetRelativeRotation(meshRot1);
	FRotator meshRot2 = TornadoMesh2->GetRelativeRotation();
	meshRot2.Yaw += -10;
	TornadoMesh2->SetRelativeRotation(meshRot2);

	FVector TornadoCurLocation = GetActorLocation();
	if (FVector::Dist(TornadoStartLocation, TornadoCurLocation) >= 700)
	{
		ObjectPoolManager->ReturnTornado(this);
	}
}

void ACTornado::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		Enemy->MyDamage(OwnerYasuo->GetDamage());
	}
}
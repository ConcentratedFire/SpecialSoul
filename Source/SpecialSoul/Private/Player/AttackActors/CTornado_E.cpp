// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AttackActors/CTornado_E.h"

#include "Components/BoxComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Item/CBaseItem.h"
#include "Net/UnrealNetwork.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CYasuo.h"

// Sets default values
ACTornado_E::ACTornado_E()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	TornadoBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TornadoBox"));
	TornadoBox->SetBoxExtent(FVector(80, 80, 32));
	RootComponent = TornadoBox;

	TornadoMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TornadoMesh1"));
	TornadoMesh1->SetupAttachment(TornadoBox);
	TornadoMesh1->SetRelativeLocation(FVector(0, 0, -30));
	TornadoMesh1->SetRelativeScale3D(FVector(1, 1, 0.07));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tmpTornado(
		TEXT("/Script/Engine.StaticMesh'/Game/Asset/Tornado/Mesh/SM_Circle_size04.SM_Circle_size04'"));
	if (tmpTornado.Succeeded())
	{
		TornadoMesh1->SetStaticMesh(tmpTornado.Object);
	}

	TornadoBox->SetCollisionProfileName(FName("PlayerAttack"));
	TornadoMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TornadoBox->OnComponentBeginOverlap.AddDynamic(this, &ACTornado_E::OnCompBeginOverlap);
}

// Called when the game starts or when spawned
void ACTornado_E::BeginPlay()
{
	Super::BeginPlay();
	OwnerYasuo = Cast<ACYasuo>(GetOwner());
}

// Called every frame
void ACTornado_E::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!OwnerYasuo) return;
	if (!OwnerYasuo->IsLocallyControlled()) return;
	SRPC_MoveTornado();
}

void ACTornado_E::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	if (HasAuthority() && !bNewHidden)
	{
		AppendYaw = 0.f;

		FVector UpLocation = GetActorLocation();
		UpLocation.Z += TornadoBox->GetScaledBoxExtent().Z;
		SetActorLocation(UpLocation);
		Damage = OwnerYasuo->GetDamage(IsCri);
	}
}

void ACTornado_E::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (auto Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		// Enemy->MyDamage(Damage);
		OwnerYasuo->MyApplyDamage(Damage, Enemy);
	}
	else if (auto Item = Cast<ACBaseItem>(OtherActor))
	{
		if (Item->GetActorNameOrLabel().Contains("ItemBox"))
			Item->ActiveItem();
	}
}

void ACTornado_E::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTornado_E, Damage);
}

void ACTornado_E::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	OwnerYasuo = Cast<ACYasuo>(NewOwner);
}

void ACTornado_E::SRPC_MoveTornado_Implementation()
{
	SetActorRotation(GetActorRotation() + Rot);
	AppendYaw += Rot.Yaw;
	if (AppendYaw >= 360)
		ObjectPoolManager->ReturnTornadoE(this);
}

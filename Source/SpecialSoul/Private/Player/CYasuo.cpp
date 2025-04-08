// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYasuo.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Anim/CYasuoAnim.h"
#include "Player/AttackActors/CTornado.h"
#include "Utility/CDataSheetUtility.h"

ACYasuo::ACYasuo()
{	
}

void ACYasuo::BeginPlay()
{
	Super::BeginPlay();
	
	// TODO Init Data Settings
	// 캐릭터를 선택하면 GameMode에서 해당 캐릭터의 정보를 읽고
	// Player의 BeginPlay에서 초기 데이터를 세팅해주도록 변경
	UCDataSheetUtility* DataSheetUtility = NewObject<UCDataSheetUtility>();
	if (DataSheetUtility)
		DataSheetUtility->FetchGoogleSheetData("Yasuo", "B2", "H7");
	// 리소스 해제
	DataSheetUtility->ConditionalBeginDestroy();
	DataSheetUtility = nullptr;

	Anim = Cast<UCYasuoAnim>(GetMesh()->GetAnimInstance());

	FTimerHandle TimerHandle;
	// GetWorldTimerManager().SetTimer(TimerHandle, [&](){Anim->PlayAttackMontage();}, 3.f, true);
}

void ACYasuo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (MP >= 100)
	// {
	// 	MP = 0;
	// }
}

void ACYasuo::Attack()
{
	TArray<FVector> AttackVectors = GetAttackVector();
	// Test
	for (const FVector& Vector : AttackVectors)
	{
		FTransform Transform;
		FVector curLocation = GetActorLocation();
		Transform.SetLocation(FVector(curLocation.X,curLocation.Y, 0));
		Transform.SetRotation(FQuat::Identity);
		Transform.SetScale3D(FVector(1.f));
		ACTornado* Tornado = GetWorld()->SpawnActorDeferred<ACTornado>(TornadoFactory, Transform, GetOwner());
		Tornado->TornadoDirection = Vector;
		UGameplayStatics::FinishSpawningActor(Tornado, Transform);
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Vector * 100.f, FColor::Red, false, 0.f, 0,
		              10.f);
	}
}

TArray<FVector> ACYasuo::GetAttackVector()
{
	FVector forwardVec = GetActorForwardVector();
	FVector Velocity = GetVelocity().GetSafeNormal();

	FVector Dir = forwardVec;
	if (Velocity != FVector::ZeroVector)
	{
		Dir = (forwardVec + Velocity) * 0.5f;
	}
	Dir.Normalize();

	float AngleStep = 360.f / static_cast<float>(AttackCnt);
	TArray<FVector> AttackVectors;

	for (int32 i = 0; i < AttackCnt; ++i)
	{
		float AngleOffset = -AngleStep * (AttackCnt - 1) / 2 + AngleStep * i;
		FRotator RotationOffset(0.0f, AngleOffset, 0.0f);
		FVector RotatedVector = RotationOffset.RotateVector(Dir);
		AttackVectors.Add(RotatedVector);
	}

	return AttackVectors;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/SkillComponent.h"

#include "EnhancedInputComponent.h"
#include "Player/CBasePlayer.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	ConstructorHelpers::FObjectFinder<UInputAction> tempESkill(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_ESkill.IA_ESkill'"));
	if (tempESkill.Succeeded())
	{
		IA_ESkill = tempESkill.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempRSkill(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_RSkill.IA_RSkill'"));
	if (tempRSkill.Succeeded())
	{
		IA_RSkill = tempRSkill.Object;
	}
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner())
	{
		OwnerCharacter = Cast<ACBasePlayer>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->OnInputBindingDel.AddUObject(this, &USkillComponent::SetInputBinding);
		}
	}
}

void USkillComponent::CastSkill(ESkillKey Key)
{
	if (!SkillMap.Contains(Key))
	{
		UE_LOG(LogTemp, Error, TEXT("!SkillMap.Contains(Key) "));
		return;
	}
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("!OwnerCharacter"));
		return;
	}

	if ((Key == ESkillKey::E || Key == ESkillKey::R) && (bUseESkill || bUseRSkill)) return; // 스킬 사용중에는 다른 스킬 사용 방지

	SkillMap[Key]->UseSkill(OwnerCharacter); // OwnerCharacter데이터를 반영해서 스킬 사용
	
	// UseSkillCount++;
}

void USkillComponent::SRPC_CastSkill_Implementation(ESkillKey Key)
{
	if (!SkillMap.Contains(Key))
	{
		UE_LOG(LogTemp, Error, TEXT("!SkillMap.Contains(Key) "));
		return;
	}
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("!OwnerCharacter"));
		return;
	}

	if ((Key == ESkillKey::E || Key == ESkillKey::R) && (bUseESkill || bUseRSkill)) return; // 스킬 사용중에는 다른 스킬 사용 방지

	SkillMap[Key]->UseSkill(OwnerCharacter); // OwnerCharacter데이터를 반영해서 스킬 사용
}

void USkillComponent::BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill)
{
	SkillMap.Add(Key, Skill);
}

void USkillComponent::SetInputBinding(class UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_ESkill, ETriggerEvent::Started, this, &USkillComponent::OnESkillPressed);
	Input->BindAction(IA_RSkill, ETriggerEvent::Started, this, &USkillComponent::OnRSkillPressed);
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillComponent::Attack()
{
	CastSkill(ESkillKey::Attack);
}

void USkillComponent::Passive()
{
	CastSkill(ESkillKey::Passive);
}

void USkillComponent::OnESkillPressed()
{
	// CastSkill(ESkillKey::E);
	SRPC_CastSkill(ESkillKey::E);
}

void USkillComponent::OnRSkillPressed()
{
	CastSkill(ESkillKey::R);
}

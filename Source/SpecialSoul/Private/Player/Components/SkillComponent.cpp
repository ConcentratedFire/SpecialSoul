// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/SkillComponent.h"

#include "EnhancedInputComponent.h"
#include "Interface/SkillStrategy.h"
#include "Player/CBasePlayer.h"

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
		OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (auto player = Cast<ACBasePlayer>(OwnerCharacter))
		{
			player->OnInputBindingDel.AddUObject(this, &USkillComponent::SetInputBinding);
		}
	}
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 쿨타임
	UpdateCooltime(DeltaTime);
}


void USkillComponent::UpdateCooltime(float deltaTime)
{
	for (auto& timer : LeftCoolTimeMap)
	{
		if (timer.Value > 0.f)
		{
			timer.Value = FMath::Max(0.f, timer.Value - deltaTime);
		}
	}
}

bool USkillComponent::ResetLeftCooltime(ESkillKey key)
{
	if (!LeftCoolTimeMap.Contains(key) || !CoolTimeMap.Contains(key) || !SkillMap.Contains(key))
	{
		return false;
	}
	
	LeftCoolTimeMap[key] = CoolTimeMap[key];
	return true;
}

bool USkillComponent::CanUseSkill(ESkillKey key)
{
	if (!LeftCoolTimeMap.Contains(key) || !CoolTimeMap.Contains(key) || !SkillMap.Contains(key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not Use Skill... "));
		return false;
	}

	return LeftCoolTimeMap[key] <= 0.f;
}


void USkillComponent::CastSkill(ESkillKey Key)
{
	//if (!CanUseSkill(Key))
	//{
	//	return;
	//}
	
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
	if (IA_ESkill)
		Input->BindAction(IA_ESkill, ETriggerEvent::Started, this, &USkillComponent::OnESkillPressed);
	if (IA_RSkill)
		Input->BindAction(IA_RSkill, ETriggerEvent::Started, this, &USkillComponent::OnRSkillPressed);
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
	CastSkill(ESkillKey::E);
}

void USkillComponent::OnRSkillPressed()
{
	CastSkill(ESkillKey::R);
}

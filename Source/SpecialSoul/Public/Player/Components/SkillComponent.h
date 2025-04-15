// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


class ACBasePlayer;
class ISkillStrategy;
enum class ESkillKey : uint8;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECIALSOUL_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACBasePlayer> OwnerCharacter {nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, TScriptInterface<ISkillStrategy>> SkillMap;

	void Attack(); // 기본공격

	void Passive();

	UFUNCTION()
	void OnESkillPressed(); // 키입력 이벤트
	
	UFUNCTION()
	void OnRSkillPressed(); // 키입력 이벤트

	 // 플레이어의 키 입력에 따른 스킬 캐스팅
	void BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill);

private:
	UFUNCTION()
	void SetInputBinding(class UEnhancedInputComponent* Input);
	
	void CastSkill(ESkillKey Key);
	
	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_ESkill;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_RSkill;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSMComponent.generated.h"

class IPathFindingStrategy;
class ABaseEnemy;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Die
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChange, EEnemyState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECIALSOUL_API UEnemyFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFSMComponent();
	void InitPathFindingStrategy(TScriptInterface<IPathFindingStrategy> InPFStrategy);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ABaseEnemy> OwnerEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	FVector TargetLocation;
	
	//FOnStateChange OnStateChange;
	// FOnAttack OnAttack;
	// FOnDamaged OnDamaged;
	// FOnDie OnDie;
	
protected:
	virtual void MoveTick(float DeltaTime);
	virtual void AttackTick(float DeltaTime);
	virtual void DieTick(float DeltaTime);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	float ElapsedTime {0.0f}; // 현재 상태에서의 경과시간

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	float AttackRange {1000.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	float AttackSpeed {4.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	float MaxHP {200.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	float HP { MaxHP };

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM", meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IPathFindingStrategy> PFStrategy;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM", meta = (AllowPrivateAccess = "true"))
	EEnemyState CurrentState { EEnemyState::Idle };

public: // GET SET
	virtual void SetState(EEnemyState NewState);
	FORCEINLINE EEnemyState GetState() const { return CurrentState; }
};

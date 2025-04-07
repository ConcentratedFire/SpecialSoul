#include "Player/Anim/CYasuoAnim.h"

#include "Player/CYasuo.h"

void UCYasuoAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACYasuo>(TryGetPawnOwner());
}

void UCYasuoAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner) return;
	Speed = Owner->GetVelocity().Size();
}

void UCYasuoAnim::PlayAttackMontage()
{
	if (!AttackMontage || !Owner) return;
	if (Speed < KINDA_SMALL_NUMBER)
		Owner->PlayAnimMontage(AttackMontage, 1, FName("IdleAttack"));
	else
		Owner->PlayAnimMontage(AttackMontage, 1, FName("MoveAttack"));
}

void UCYasuoAnim::AnimNotify_YasuoDefaultAttack()
{
	Owner->Attack();
}

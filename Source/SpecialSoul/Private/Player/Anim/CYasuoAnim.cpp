#include "Player/Anim/CYasuoAnim.h"

#include "SpecialSoul.h"
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

void UCYasuoAnim::PlayESkillMontage(bool bAnimStart)
{
	if (!ESkillMontage || !Owner) return;
	// LOG_S(Warning, TEXT("ESkill : %d"), bAnimStart);
	if (bAnimStart)
		Owner->PlayAnimMontage(ESkillMontage, 1, FName("DashStart"));
	else
		Owner->PlayAnimMontage(ESkillMontage, 1, FName("Attack"));
}

void UCYasuoAnim::PlayRSkillMontage()
{
	if (!RSkillMontage || !Owner) return;
	Owner->PlayAnimMontage(RSkillMontage, 1, FName("WindWall"));
}

void UCYasuoAnim::PlayDieAnimation()
{
	Montage_Play(DieAnim);
}

void UCYasuoAnim::AnimNotify_YasuoDefaultAttack()
{
	if (Owner && !Owner->HasAuthority()) return;
	Owner->Attack();
}

void UCYasuoAnim::AnimNotify_ESkillEnd()
{
	Owner->ActivateSkillMovement(false);
	Owner->SetSkillUsing(ESkillKey::E, false);
}

void UCYasuoAnim::AnimNotify_WindWallSkill()
{
	Owner->WindWall();
	Owner->ActivateSkillMovement(false);
	Owner->SetSkillUsing(ESkillKey::R, false);
}

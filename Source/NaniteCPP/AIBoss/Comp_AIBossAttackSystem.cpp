
#include "Comp_AIBossAttackSystem.h"
#include "BossAttackStructData.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#define AddDynamic( UserObject, FuncName ) __Internal_AddDynamic( UserObject, FuncName, STATIC_FUNCTION_FNAME( TEXT( #FuncName ) ) )

UComp_AIBossAttackSystem::UComp_AIBossAttackSystem()
{
	PrimaryComponentTick.bCanEverTick = false;


	TempCharacter = Cast<ACharacter>(GetOwner());
	// ...
}


// Called when the game starts
void UComp_AIBossAttackSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

//=========================BossSkill==================================
//=========================BossSkill==================================
//=========================BossSkill==================================
void UComp_AIBossAttackSystem::BossPrimaryAttack(FBOSSATTACKDATA AttackInfo)
{
	currentInfo = AttackInfo;

	if (TempCharacter) {
		UAnimInstance* AnimInstance = TempCharacter->GetMesh()->GetAnimInstance(); //캐릭터에 애니메이션 blueprint가 설정되어 있어야 한다.

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *AnimInstance->GetName());

		if (AnimInstance && currentInfo.Montage) {
			AnimInstance->Montage_Play(currentInfo.Montage);
			//notify 가 시작되었을 때
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossPrimary);
		}
	}
}

void UComp_AIBossAttackSystem::OnNotifyBossPrimary(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{

	if (NotifyName == "Slash") {
		UE_LOG(LogTemp, Warning, TEXT("Slash"));
		SphereTraceDamage(currentInfo);
	}
	if (NotifyName == "AOESlash") {
		UE_LOG(LogTemp, Warning, TEXT("AOESlash"));
	}

}

void UComp_AIBossAttackSystem::BossThrowBall(FBOSSATTACKDATA AttackInfo)
{
	currentInfo = AttackInfo;

	if (TempCharacter) {
		UAnimInstance* AnimInstance = TempCharacter->GetMesh()->GetAnimInstance(); //캐릭터에 애니메이션 blueprint가 설정되어 있어야 한다.

		if (AnimInstance && currentInfo.Montage) {
			AnimInstance->Montage_Play(currentInfo.Montage);
			//notify 가 시작되었을 때
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossThrowBall);
		}
	}
}

void UComp_AIBossAttackSystem::OnNotifyBossThrowBall(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Throw") {
		UE_LOG(LogTemp, Warning, TEXT("Throw"));
		SphereTraceDamage(currentInfo);
	}
}

void UComp_AIBossAttackSystem::BossJumpAttack(FBOSSATTACKDATA AttackInfo)
{
	currentInfo = AttackInfo;
}

void UComp_AIBossAttackSystem::OnNotifyBossJumpAttack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{

}




//=========================BossSkill==================================
//=========================BossSkill==================================
//=========================BossSkill==================================


//=========================ApplyDamage==================================
//=========================ApplyDamage==================================
//=========================ApplyDamage==================================
void UComp_AIBossAttackSystem::SphereTraceDamage(FBOSSATTACKDATA cInfo)
{
	FHitResult OutHit;
	TArray<AActor*> IgnoredActors;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorForwardVector() * cInfo.length,
		cInfo.radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		IgnoredActors,
		EDrawDebugTrace::Persistent,
		OutHit,
		true
	);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *OutHit.GetActor()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	}

}
//=========================ApplyDamage==================================
//=========================ApplyDamage==================================
//=========================ApplyDamage==================================


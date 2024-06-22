
#include "Comp_AIBossAttackSystem.h"
#include "BossAttackStructData.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "BossHomingBall.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"

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
			UE_LOG(LogTemp, Warning, TEXT("%s"), *currentInfo.Montage->GetName());
			AnimInstance->Montage_Play(currentInfo.Montage);
			//notify 가 시작되었을 때
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossThrowBall);
		}
	}
}

void UComp_AIBossAttackSystem::OnNotifyBossThrowBall(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{

	if (NotifyName == "SpawnBall") {
		UE_LOG(LogTemp, Warning, TEXT("SpawnBall"));
		if (BossThrowBallActor) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector loc = GetOwner()->GetActorLocation();
			loc.Z += 200.f;
			ABossHomingBall* tempHB = GetWorld()->SpawnActor<ABossHomingBall>(BossThrowBallActor, loc, { 0,0,0 }, ActorSpawnParams);
			HomingBalls.Add(tempHB);

			//ANaniteCPPCharacter* hero = Cast<ANaniteCPPCharacter>(play)
			//tempHB->D_Hit.AddUObject(this,&UComp_AIBossAttackSystem::)
		
			//spawn하면서 인자를 전달하는 방법
			//ABossHomingBall* SpawnedBall = GetWorld()->SpawnActorDeferred<ABossHomingBall>(BossThrowBallActor, 
			//	FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation()), 
			//	nullptr, 
			//	nullptr, 
			//	ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			//if (SpawnedBall) {
			//	SpawnedBall->Target = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn(); // 예: 특정 속성에 값을 설정
			//	if (SpawnedBall->Target) {
			//		UE_LOG(LogTemp,Warning,TEXT("%s"),*SpawnedBall->Target->GetActorLocation().ToString())
			//		UGameplayStatics::FinishSpawningActor(SpawnedBall, FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation()));
			//	}
			//}
		}
	}
	if (NotifyName == "Throw") {
		UE_LOG(LogTemp, Warning, TEXT("Throw"));
		for (ABossHomingBall* a : HomingBalls) {
			UProjectileMovementComponent* tempP =  a->ProjectileMovement;
			tempP->HomingAccelerationMagnitude = 3000.f;
		}
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


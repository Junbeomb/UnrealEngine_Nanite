
#include "Comp_AIBossAttackSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../BeginnerCharacter/NaniteCPPCharacter.h"
#include "../Skills/ThrowBall/BossThrowBall.h"
#include "BossBase.h"
#include "../Skills/Meteor/MeteorChargeCenter.h"
#include "../Skills/Meteor/MeteorRock.h"

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
		//UE_LOG(LogTemp, Warning, TEXT("%s"),*currentInfo.Montage->GetName());

		if (AnimInstance && currentInfo.Montage) {
			AnimInstance->Montage_Play(currentInfo.Montage);

			//notify 가 시작되었을 때
			switch (currentInfo.CurrentSkill) {
			case EBossSkill::Combo1:
				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossCombo1);
				break;
			case EBossSkill::Jump:
				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossJumpAttack);
				break;
			case EBossSkill::ThrowBall:
				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossThrowBall);
				break;
			case EBossSkill::Meteor:
				UE_LOG(LogTemp, Warning, TEXT("MeteorAttackNotifyBegin"));
				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossMeteorAttack);
				break;
			case EBossSkill::Final:
				UE_LOG(LogTemp, Warning, TEXT("FinalAttackNotifyBegin"));
				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UComp_AIBossAttackSystem::OnNotifyBossFinalAttack);
				break;
			}
		}
	}

}

void UComp_AIBossAttackSystem::OnNotifyBossCombo1(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Slash") {
		UE_LOG(LogTemp, Warning, TEXT("Slash"));
	}
	if (NotifyName == "AOESlash") {
		UE_LOG(LogTemp, Warning, TEXT("AOESlash"));
	}
}

void UComp_AIBossAttackSystem::OnNotifyBossJumpAttack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Jump") {
		UE_LOG(LogTemp, Warning, TEXT("Jump"));

		//LaunchCharacFunc();
		FVector OutLaunchVelocity;

		bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
			this,
			OutLaunchVelocity,
			GetOwner()->GetActorLocation(),
			currentInfo.AttackTarget->GetActorLocation(),
			0.f,
			0.5f
		);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *OutLaunchVelocity.ToString());
		TempCharacter->LaunchCharacter(OutLaunchVelocity, true, true);

	}
	if (NotifyName == "GroundSmash") {
		UE_LOG(LogTemp, Warning, TEXT("GroundSmash"));
	}
}

void UComp_AIBossAttackSystem::OnNotifyBossThrowBall(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{

	if (NotifyName == "SpawnBall") {
		UE_LOG(LogTemp, Warning, TEXT("SpawnBall"));
		if (ThrowBallChoice) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector loc = GetOwner()->GetActorLocation();
			loc.Z += 200.f;
			ABossThrowBall* tempHB = GetWorld()->SpawnActor<ABossThrowBall>(ThrowBallChoice, loc, { 0,0,0 }, ActorSpawnParams);
			ThrowBalls.Add(tempHB);

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
		for (ABossThrowBall* a : ThrowBalls) {
			/*UProjectileMovementComponent* tempP =  a->ProjectileMovement;
			tempP->HomingAccelerationMagnitude = 3000.f;*/
		}
	}
}

void UComp_AIBossAttackSystem::OnNotifyBossMeteorAttack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Charging") {
		//UE_LOG(LogTemp, Warning, TEXT("Charging"));

		ABossBase* boss = Cast<ABossBase>(TempCharacter);
		if (boss->HammerOverlayMaterial) {
			UStaticMeshComponent* HammerComp = Cast<UStaticMeshComponent>(boss->HammerActor->GetChildActor()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			HammerComp->SetOverlayMaterial(boss->HammerOverlayMaterial);
		}
	}
	if (NotifyName == "Strike") {
		//UE_LOG(LogTemp, Warning, TEXT("Strike"));

		float MinValue = -300.0f;
		float MaxValue = 300.0f;
		float randomX = GetOwner()->GetActorLocation().X + FMath::RandRange(MinValue, MaxValue);
		float randomY = GetOwner()->GetActorLocation().Y + FMath::RandRange(MinValue, MaxValue);

		SphereTraceDamage(currentInfo,{ randomX ,randomY,0}, { randomX ,randomY,0 });

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		MeteorRocks.Add(GetWorld()->SpawnActor<AMeteorRock>(MeteorRockChoice, { randomX ,randomY,0 }, { 0,0,0 }, ActorSpawnParams));
	}
	if (NotifyName == "SpawnCenter") {
		//UE_LOG(LogTemp, Warning, TEXT("SpawnCenter"));
		if (MeteorCenterChoice) {

			FVector SpawnLocation = GetOwner()->GetActorLocation();
			SpawnLocation.Z += 300.f;

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			MeteorCenter = GetWorld()->SpawnActor<AMeteorChargeCenter>(MeteorCenterChoice, SpawnLocation, {0,0,0}, ActorSpawnParams);

			for (AMeteorRock* a : MeteorRocks) {
				a->MeteorCenterBind();
			}
		}
	}
	if (NotifyName == "AbsortSoul") {
		//UE_LOG(LogTemp, Warning, TEXT("AbsortSoul"));

		MeteorCenter->CallDAbsortStart();
	}
	if (NotifyName == "Bomb") {
		//UE_LOG(LogTemp, Warning, TEXT("Bomb"));

		MeteorCenter->CallDBombStart();
	}
}

void UComp_AIBossAttackSystem::OnNotifyBossFinalAttack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Spawn") {
		UE_LOG(LogTemp, Warning, TEXT("Spawn"));

	}
	if (NotifyName == "ChargingEnd") {
		UE_LOG(LogTemp, Warning, TEXT("ChargingEnd"));
	}
}


//=========================BossSkill==================================
//=========================BossSkill==================================
//=========================BossSkill==================================


//=========================ApplyDamage==================================
//=========================ApplyDamage==================================
//=========================ApplyDamage==================================
void UComp_AIBossAttackSystem::SphereTraceDamage(FBOSSATTACKDATA cInfo, FVector startLocation, FVector endLocation)
{
	FHitResult OutHit;
	TArray<AActor*> IgnoredActors;

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		startLocation,
		endLocation,
		cInfo.radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
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

void UComp_AIBossAttackSystem::CapsuleTraceDamage(FBOSSATTACKDATA cInfo, FVector startLocation, FVector endLocation)
{
	FHitResult OutHit;
	TArray<AActor*> IgnoredActors;

	bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		startLocation,
		endLocation,
		cInfo.radius,
		cInfo.length,
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


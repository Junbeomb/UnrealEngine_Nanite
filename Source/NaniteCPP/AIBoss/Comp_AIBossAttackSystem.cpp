
#include "Comp_AIBossAttackSystem.h"
#include "BossAttackStructData.h"
#include "GameFramework/Character.h"

UComp_AIBossAttackSystem::UComp_AIBossAttackSystem()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UComp_AIBossAttackSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

void UComp_AIBossAttackSystem::BossPrimaryAttack(FBOSSATTACKDATA AttackInfo, float Radius, float Length)
{
	ACharacter* TempCharacter = Cast<ACharacter>(GetOwner());
	if (TempCharacter) {
		TempCharacter->GetMesh()->PlayAnimation(AttackInfo.Montage, false);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *AttackInfo.Montage->GetName());
	}

}

void UComp_AIBossAttackSystem::BossThrowBall()
{
}

void UComp_AIBossAttackSystem::BossJumpAttack()
{
}


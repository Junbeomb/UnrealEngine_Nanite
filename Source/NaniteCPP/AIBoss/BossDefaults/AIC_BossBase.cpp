// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_BossBase.h"
#include "BossBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Damage.h"

AAIC_BossBase::AAIC_BossBase()
{
	BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 3000.f;
	SightConfig->LoseSightRadius = 6000.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
}

void AAIC_BossBase::OnPossess(APawn* InPawn)
{
	BossBase = Cast<ABossBase>(InPawn);
	if (IsValid(BossBase->BehaviorTree) && BlackBoardComp) {
		RunBehaviorTree(BossBase->BehaviorTree);
		SetStateAsPassive();
	}
}

EBossState AAIC_BossBase::GetCurrentState()
{
	return static_cast<EBossState>(BlackBoardComp->GetValueAsEnum(FName("State")));
}

void AAIC_BossBase::SetStateAsPassive()
{
	BlackBoardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(EBossState::Passive));
	UE_LOG(LogTemp, Warning, TEXT("Passive!"));
}

void AAIC_BossBase::SetStateAsAttacking(AActor* ATarget, bool useLastKnownAttackTarget)
{ 
	AActor* NewAttackTarget;
	//이전 Target이 있으면
	if (useLastKnownAttackTarget && AttackTarget) {
		NewAttackTarget = AttackTarget;
	}
	else {
		NewAttackTarget = ATarget;
	}

	//공격
	if (IsValid(NewAttackTarget)) {
		BlackBoardComp->SetValueAsObject(FName("AttackTarget"), NewAttackTarget);
		BlackBoardComp->SetValueAsEnum(FName("State"),static_cast<uint8>(EBossState::Attack));
	}
	else {
		SetStateAsPassive();
		return;
	}

	//타겟 설정
	AttackTarget = NewAttackTarget;
}

void AAIC_BossBase::SetStateAsFrozen()
{
	BlackBoardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(EBossState::Frozen));
}

void AAIC_BossBase::SetStateAsDeath()
{
	BlackBoardComp->SetValueAsEnum(FName("State"), static_cast<uint8>(EBossState::Death));
}

void AAIC_BossBase::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* a : UpdatedActors) {
		if (CanSenseActor(a, EBossSenseType::Sight)) {
			UE_LOG(LogTemp, Warning, TEXT("Sight!!!!"));
		}
		if (CanSenseActor(a, EBossSenseType::Damage)) {
			UE_LOG(LogTemp, Warning, TEXT("Damage!!!!"));
		}
	}
}

bool AAIC_BossBase::CanSenseActor(AActor* actor, EBossSenseType sensetype)
{
	FActorPerceptionBlueprintInfo tempInfo;
	AIPerceptionComponent->GetActorsPerception(actor, tempInfo);

	UClass* SaveWhatSense = nullptr;
	switch (sensetype) {
	case EBossSenseType::None:
		SaveWhatSense = nullptr;
		break;
	case EBossSenseType::Sight:
		SaveWhatSense = UAISense_Sight::StaticClass();
		break;
	case EBossSenseType::Damage:
		SaveWhatSense = UAISense_Damage::StaticClass();
		break;
	}

	//자극 받은 sense가 sight인지 damage 인지 확인
	for (FAIStimulus Sti : tempInfo.LastSensedStimuli) {

		UClass* SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Sti);

		if (SaveWhatSense && SenseClass == SaveWhatSense) {
			return true;
		}
	}

	return false;
}

void AAIC_BossBase::HandleSensedSight(AActor* actor)
{
	KnownSeenActors.AddUnique(actor);
	EBossState tempState = GetCurrentState();

	switch (tempState) {
	case EBossState::Passive:
		SetStateAsAttacking(actor, false);
		break;
	case EBossState::Attack:
		break;
	case EBossState::Frozen:
		break;
	case EBossState::Death:
		break;
	}
}


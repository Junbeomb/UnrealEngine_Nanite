// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_BossBase.h"
#include "AIBossBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"




AAIC_BossBase::AAIC_BossBase()
{
	BlackBoardComp->CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
}

void AAIC_BossBase::OnPossess(APawn* InPawn)
{
	BossBase = Cast<AAIBossBase>(InPawn);
	if (IsValid(BossBase->BehaviorTree) && BlackBoardComp) {
		RunBehaviorTree(BossBase->BehaviorTree);
		SetStateAsPassive();
	}
}

void AAIC_BossBase::SetStateAsPassive()
{
	BlackBoardComp->SetValueAsEnum(TEXT("State"), static_cast<uint8>(EBossState::Passive));

}

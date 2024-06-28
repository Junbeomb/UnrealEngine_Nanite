// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "../Datas/E_BossState.h"

#include "AIC_BossBase.generated.h"

/**
 * 
 */
UCLASS()
class NANITECPP_API AAIC_BossBase : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class ABossBase* BossBase;

	UPROPERTY(EditAnywhere)
	EBossState BState;

	UPROPERTY(EditAnywhere)
	class UBlackboardComponent* BlackBoardComp;

	UPROPERTY(EditAnywhere)
	AActor* AttackTarget;

	UPROPERTY(EditAnywhere)
	class UAIPerceptionComponent* AIPerceptionComponent;

	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	bool CanSenseActor(AActor* actor, EBossSenseType sensetype);

	UFUNCTION()
	void HandleSensedSight(AActor* actor);

	UPROPERTY(EditAnywhere)
	TArray<AActor*> KnownSeenActors;


public:
	AAIC_BossBase();

	UFUNCTION()
	EBossState GetCurrentState();


	UFUNCTION()
	void SetStateAsPassive();

	UFUNCTION()
	void SetStateAsAttacking(AActor* ATarget, bool useLastKnownAttackTarget);

	UFUNCTION()
	void SetStateAsFrozen();

	UFUNCTION()
	void SetStateAsDeath();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};

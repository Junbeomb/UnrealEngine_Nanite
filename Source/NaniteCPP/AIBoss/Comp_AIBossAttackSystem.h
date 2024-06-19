// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossAttackStructData.h"
#include "Comp_AIBossAttackSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NANITECPP_API UComp_AIBossAttackSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComp_AIBossAttackSystem();

	UFUNCTION()
	void BossPrimaryAttack(FBOSSATTACKDATA AttackInfo);

	UPROPERTY(EditAnywhere)
	FBOSSATTACKDATA currentInfo;

	UFUNCTION()
	void OnNotifyBossPrimary(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void BossThrowBall();
	UFUNCTION()
	void BossJumpAttack();


	UFUNCTION()
	void SphereTraceDamage(FBOSSATTACKDATA cInfo);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};

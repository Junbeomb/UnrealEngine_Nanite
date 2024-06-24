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

	ACharacter* TempCharacter;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BossThrowBallActor;

	UPROPERTY(EditAnywhere)
	TArray<class ABossHomingBall*> HomingBalls;


public:	
	// Sets default values for this component's properties
	UComp_AIBossAttackSystem();

	UPROPERTY(EditAnywhere)
	FBOSSATTACKDATA currentInfo;

	UFUNCTION()
	void BossPrimaryAttack(FBOSSATTACKDATA AttackInfo);
	UFUNCTION()
	void OnNotifyBossCombo1(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnNotifyBossThrowBall(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnNotifyBossJumpAttack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnNotifyBossMeteorAttack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void SphereTraceDamage(FBOSSATTACKDATA cInfo);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "BossBase.generated.h"

UCLASS()
class NANITECPP_API ABossBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;



	UPROPERTY(EditAnywhere)
	class UComp_AIDamageSystem* Comp_Damage;
	UPROPERTY(EditAnywhere)
	class UComp_BlendMesh* Comp_Blend;

	UPROPERTY(EditAnywhere)
	class AAIController* AIC_BossBase;



	UFUNCTION()
	void Die();

	UFUNCTION()
	void HitResponse();

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInstanceDynamic*> DMIList;


	//=====================DissolveTimeline=========================
	//=====================DissolveTimeline=========================
	//=====================DissolveTimeline=========================
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* DissolveTimeline;

	//DissolveTimeline 종료시
	class FOnTimelineEvent floatTimelineFinishedCallback;
	UFUNCTION()
	void DissolveTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* DissolveCurve;
	//callback함수
	class FOnTimelineFloat floatTimelineCallback;
	//Bind함수
	UFUNCTION()
	void DissolveTimelineUpdate(float Value);

	//=====================DissolveTimeline=========================
	//=====================DissolveTimeline=========================
	//=====================DissolveTimeline=========================

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Combo1Montage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* ThrowBallMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* JumpAttackMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* MeteorAttackMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* FinalAttackMontage;

public:

	ABossBase();

	UPROPERTY(EditAnywhere, Category = "ChildActor")
	UChildActorComponent* HammerActor;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* HammerOverlayMaterial;

	UPROPERTY(EditAnywhere)
	class UComp_AIBossAttackSystem* Comp_Attack;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UFUNCTION()
	void AttackCombo1(AActor* ATarget);

	UFUNCTION()
	void ThrowBall(AActor* ATarget);

	UFUNCTION()
	void JumpAttack(AActor* ATarget);


	UFUNCTION()
	void MeteorAttack(AActor* ATarget);

	UFUNCTION()
	void FinalAttack(AActor* ATarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

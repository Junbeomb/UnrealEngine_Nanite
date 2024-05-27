// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "AIBossBase.generated.h"

UCLASS()
class NANITECPP_API AAIBossBase : public ACharacter
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
	class UBehaviorTree* BehaviorTree;

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

public:
	// Sets default values for this character's properties
	AAIBossBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

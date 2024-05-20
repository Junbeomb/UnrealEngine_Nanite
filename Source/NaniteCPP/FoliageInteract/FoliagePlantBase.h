// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoliageInfluencer.h"
#include "NiagaraComponent.h"
#include "Components/TimelineComponent.h"
#include "FoliagePlantBase.generated.h"


UCLASS()
class NANITECPP_API AFoliagePlantBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoliagePlantBase();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, category="collision")
	class USphereComponent* OverlappingSphere;
	UPROPERTY(EditAnywhere, category = "collision")
	class UCapsuleComponent* OverlappingCapsule;
	UPROPERTY(EditAnywhere,category="collision")
	bool overlapIsSphere;

	class UComp_BlendMesh* Comp_Blend;

	//���۽� ����
	UPROPERTY(EditAnywhere, category = "Sound")
	UAudioComponent* SoundEffect;
	UPROPERTY(EditAnywhere, category = "Sound")
	bool soundOn;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void ReturnToFoliage();

	//���忡 �ִ� FoliageInfluencers ������
	UPROPERTY(EditAnywhere, category = "Physics")
	TArray<AActor*> AllFoliageInfluencers;
	UPROPERTY(EditAnywhere, category = "Physics")
	int FoliageInfluencersLen;
	UPROPERTY(EditAnywhere, category = "Physics")
	bool IsInfluencersInRange;
	UFUNCTION()
	void NoInfluencersInRangeFunc();

	//���� Ÿ�Ӷ���
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* FastTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* SlowTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	bool IsSlowFoliageReturn;

	//���忡�ִ� Foliage ��������
	class AInstancedFoliageActor* WorldFoliage;

	//FoliageInfluencer�� ������ �ִ��� Ȯ��
	UFUNCTION()
	void checkToFoliageInfluencer();

	UPROPERTY()
	bool DoOnce;
	//BlendWeight�� ����.(0�̸� �ִϸ��̼�, 1�̸� Physics)
	UPROPERTY()
	float PhysicsAlpha;

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* FastCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* SlowCurve;

	//callback�Լ�
	FOnTimelineEvent floatTimelineFinishedCallback;
	//Bind�Լ�
		UFUNCTION()
		void BlendWeightTimelineFinish();
	UPROPERTY(EditAnywhere)
	bool TimelineFinishDoOnce;

	//callback�Լ�
	FOnTimelineFloat floatTimelineCallback;
	//Bind�Լ�
		UFUNCTION()
		void BlendWeightTimelineUpdate(float Value);


	//overlap ������ �����Լ�
	UFUNCTION()
	void OverlapSphereOrCapsule(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere)
	UNiagaraSystem* L_Niagara;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* H_Niagara;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "FoliagePlantBase.generated.h"


UCLASS()
class NANITECPP_API AFoliagePlantBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComponent;
	
public:	
	AFoliagePlantBase();

	UFUNCTION()
	void NoInfluencersInRangeTL();


	UPROPERTY(EditAnywhere, category="collision")
	class USphereComponent* OverlappingSphere;
	UPROPERTY(EditAnywhere, category = "collision")
	class UCapsuleComponent* OverlappingCapsule;
	UPROPERTY(EditAnywhere,category="collision")
	bool overlapIsSphere;

	//���۽� ����
	UPROPERTY(EditAnywhere, category = "Sound")
	UAudioComponent* SoundEffect;
	UPROPERTY(EditAnywhere, category = "Sound")
	bool soundOn;


private:
	//���� Ÿ�Ӷ���
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* FastTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* SlowTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	bool IsSlowFoliageReturn;


	//BlendWeight�� ����.(0�̸� �ִϸ��̼�, 1�̸� Physics)
	UPROPERTY()
	float PhysicsAlpha;

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* FastCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* SlowCurve;

	//callback�Լ�
	FOnTimelineEvent floatTimelineFinishedCallback;
	//Bind�Լ�( ��� ���� ��)
		//UFUNCTION()
		//void NoInfluencersInRangeFunc();
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
	class UNiagaraSystem* L_Niagara;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* H_Niagara;

protected:
	virtual void BeginPlay() override;

	//���忡 �ִ� FoliageInfluencers ������
	UPROPERTY(EditAnywhere, category = "Physics")
	TArray<AActor*> AllFoliageInfluencers;
	UPROPERTY(EditAnywhere, category = "Physics")
	int FoliageInfluencersLen;
	UPROPERTY(EditAnywhere, category = "Physics")
	bool IsInfluencersInRange;
	UFUNCTION()
	void NoInfluencersInRangeFunc();

	//���忡�ִ� Foliage ��������
	class AInstancedFoliageActor* WorldFoliage;

	class UComp_BlendMesh* Comp_Blend;


	UFUNCTION()
	void ReturnToFoliage();

	//FoliageInfluencer�� ������ �ִ��� Ȯ��
	UFUNCTION()
	void checkToFoliageInfluencer();

	UPROPERTY()
	bool DoOnce;

	UPROPERTY(EditAnywhere)
	bool FinishDoOnce;
};

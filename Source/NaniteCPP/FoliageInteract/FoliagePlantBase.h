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

	//시작시 사운드
	UPROPERTY(EditAnywhere, category = "Sound")
	UAudioComponent* SoundEffect;
	UPROPERTY(EditAnywhere, category = "Sound")
	bool soundOn;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void ReturnToFoliage();

	//월드에 있는 FoliageInfluencers 정보들
	UPROPERTY(EditAnywhere, category = "Physics")
	TArray<AActor*> AllFoliageInfluencers;
	UPROPERTY(EditAnywhere, category = "Physics")
	int FoliageInfluencersLen;
	UPROPERTY(EditAnywhere, category = "Physics")
	bool IsInfluencersInRange;
	UFUNCTION()
	void NoInfluencersInRangeFunc();

	//블랜드 타임라인
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* FastTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* SlowTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	bool IsSlowFoliageReturn;

	//월드에있는 Foliage 가져오기
	class AInstancedFoliageActor* WorldFoliage;

	//FoliageInfluencer가 주위에 있는지 확인
	UFUNCTION()
	void checkToFoliageInfluencer();

	UPROPERTY()
	bool DoOnce;
	//BlendWeight의 정도.(0이면 애니메이션, 1이면 Physics)
	UPROPERTY()
	float PhysicsAlpha;

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* FastCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* SlowCurve;

	//callback함수
	FOnTimelineEvent floatTimelineFinishedCallback;
	//Bind함수
		UFUNCTION()
		void BlendWeightTimelineFinish();
	UPROPERTY(EditAnywhere)
	bool TimelineFinishDoOnce;

	//callback함수
	FOnTimelineFloat floatTimelineCallback;
	//Bind함수
		UFUNCTION()
		void BlendWeightTimelineUpdate(float Value);


	//overlap 됐을시 시작함수
	UFUNCTION()
	void OverlapSphereOrCapsule(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere)
	UNiagaraSystem* L_Niagara;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* H_Niagara;
};

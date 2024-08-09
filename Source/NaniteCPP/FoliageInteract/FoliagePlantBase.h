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

	//시작시 사운드
	UPROPERTY(EditAnywhere, category = "Sound")
	UAudioComponent* SoundEffect;
	UPROPERTY(EditAnywhere, category = "Sound")
	bool soundOn;


private:
	//블랜드 타임라인
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* FastTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* SlowTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	bool IsSlowFoliageReturn;


	//BlendWeight의 정도.(0이면 애니메이션, 1이면 Physics)
	UPROPERTY()
	float PhysicsAlpha;

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* FastCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* SlowCurve;

	//callback함수
	FOnTimelineEvent floatTimelineFinishedCallback;
	//Bind함수( 상속 받을 거)
		//UFUNCTION()
		//void NoInfluencersInRangeFunc();
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
	class UNiagaraSystem* L_Niagara;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* H_Niagara;

protected:
	virtual void BeginPlay() override;

	//월드에 있는 FoliageInfluencers 정보들
	UPROPERTY(EditAnywhere, category = "Physics")
	TArray<AActor*> AllFoliageInfluencers;
	UPROPERTY(EditAnywhere, category = "Physics")
	int FoliageInfluencersLen;
	UPROPERTY(EditAnywhere, category = "Physics")
	bool IsInfluencersInRange;
	UFUNCTION()
	void NoInfluencersInRangeFunc();

	//월드에있는 Foliage 가져오기
	class AInstancedFoliageActor* WorldFoliage;

	class UComp_BlendMesh* Comp_Blend;


	UFUNCTION()
	void ReturnToFoliage();

	//FoliageInfluencer가 주위에 있는지 확인
	UFUNCTION()
	void checkToFoliageInfluencer();

	UPROPERTY()
	bool DoOnce;

	UPROPERTY(EditAnywhere)
	bool FinishDoOnce;
};

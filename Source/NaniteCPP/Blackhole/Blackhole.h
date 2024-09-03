
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "Blackhole.generated.h"

DECLARE_MULTICAST_DELEGATE(FDieEvent);

UCLASS()
class NANITECPP_API ABlackhole : public AActor
{
	GENERATED_BODY()

	float blackholeLife;
	float TL_PullFoliageRadius;
	float TL_TurnOfDFRadius;
	float TL_WindRadius;
	float TL_WindStrength;
	float TL_DFRadius;
	float TL_PullStrength;
	float TL_VortexAmount;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BlackholeBaseMesh;

	UPROPERTY(EditAnywhere, Category = "ChildActor")
	UChildActorComponent* FoliageToBPActor;

	UPROPERTY(EditAnywhere, Category = "ShpereCollision")
	class USphereComponent* TurnOffDFRange;
	UPROPERTY(EditAnywhere, Category = "ShpereCollision")
	class USphereComponent* PullRange;


	//Die �Լ�
	bool DieToggle;
	void DieBlackhole();


	//Timeline
		UTimelineComponent* MeshTimeline;
		UTimelineComponent* RangeTimeline;
		UTimelineComponent* DFStartRadiusTimeline;

		//Blackhole Mesh Size Ÿ�� ���� 
			//MeshTimeline �����
		FOnTimelineEvent floatTimelineFinishedCallback;
		UFUNCTION()
		void SetScaleTimelineFinish();

		UPROPERTY(EditAnywhere, Category = "Timeline");
		class UCurveFloat* MeshCurve;
		//callback�Լ�
		FOnTimelineFloat floatTimelineCallback;
		//Bind�Լ�
		UFUNCTION()
		void SetScaleTimelineUpdate(float Value);


		//Range Ÿ�Ӷ���
			//Range �����
		FOnTimelineEvent RangeTimelineFinishedCallback;
		UFUNCTION()
		void RangeTimelineFinish();

		//Pull
		UPROPERTY(EditAnywhere, Category = "Timeline");
		class UCurveFloat* PullRangeCurve;
		//callback�Լ�
		FOnTimelineFloat PullRangeTimelineCallback;
		//Bind�Լ�
		UFUNCTION()
		void PullRangeTimelineUpdate(float Value);

		//DF
		UPROPERTY(EditAnywhere, Category = "Timeline");
		class UCurveFloat* DFRangeCurve;
		//callback�Լ�
		FOnTimelineFloat DFRangeTimelineCallback;
		//Bind�Լ�
		UFUNCTION()
		void DFRangeTimelineUpdate(float Value);

		//Foliage
		UPROPERTY(EditAnywhere, Category = "Timeline");
		class UCurveFloat* FoliageRangeCurve;
		//callback�Լ�
		FOnTimelineFloat FoliageRangeTimelineCallback;
		//Bind�Լ�
		UFUNCTION()
		void FoliageRangeTimelineUpdate(float Value);

		//DFStartRadius Ÿ�Ӷ���
				//�����
		FOnTimelineEvent DFStartRadiusTimelineFinishedCallback;
		UFUNCTION()
		void DFStartRadiusTimelineFinish();

		//Pull
		UPROPERTY(EditAnywhere, Category = "Timeline");
		class UCurveFloat* DFStartRadiusCurve;
		//callback�Լ�
		FOnTimelineFloat DFStartRadiusTimelineCallback;
		//Bind�Լ�
		UFUNCTION()
		void DFStartRadiusTimelineUpdate(float Value);
	//Timeline


	float DFStartRadius;

	UFUNCTION()
	void OverlapPullRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapDFRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
		virtual void BeginPlay() override;

public:	
	ABlackhole();

	float GetDFStartRadius();

	FDieEvent D_SoonDie;

};

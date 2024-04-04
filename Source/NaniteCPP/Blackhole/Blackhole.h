
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Blackhole.generated.h"

UCLASS()
class NANITECPP_API ABlackhole : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABlackhole();

	UPROPERTY()
	float DFStartRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BlackholeBaseMesh;

	UPROPERTY(EditAnywhere, Category = "ChildActor")
	UChildActorComponent* FoliageToBPActor;

	UPROPERTY(EditAnywhere, Category = "ShpereCollision")
	class USphereComponent* TurnOffDFRange;
	UPROPERTY(EditAnywhere, Category = "ShpereCollision")
	class USphereComponent* PullRange;

	UPROPERTY(EditAnywhere,  Category = "Timeline")
	class UTimelineComponent* MeshTimeline;
	UPROPERTY(EditAnywhere,  Category = "Timeline")
	class UTimelineComponent* RangeTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* DFStartRadiusTimeline;

	//Die 함수
	bool DieToggle;
	UFUNCTION()
	void DieBlackhole();

	//Blackhole Mesh Size 타임 라인
		//MeshTimeline 종료시
		FOnTimelineEvent floatTimelineFinishedCallback;
			UFUNCTION()
			void SetScaleTimelineFinish();

		UPROPERTY(EditAnywhere, Category = "Timeline");
		class UCurveFloat* MeshCurve;
		//callback함수
		FOnTimelineFloat floatTimelineCallback;
			//Bind함수
			UFUNCTION()
			void SetScaleTimelineUpdate(float Value);


	//Range 타임라인
		//Range 종료시
		FOnTimelineEvent RangeTimelineFinishedCallback;
		UFUNCTION()
		void RangeTimelineFinish();

		//Pull
		UPROPERTY(EditAnywhere,  Category = "Timeline");
		class UCurveFloat* PullRangeCurve;
		//callback함수
		FOnTimelineFloat PullRangeTimelineCallback;
		//Bind함수
			UFUNCTION()
			void PullRangeTimelineUpdate(float Value);

		//DF
		UPROPERTY(EditAnywhere,  Category = "Timeline");
		class UCurveFloat* DFRangeCurve;
		//callback함수
		FOnTimelineFloat DFRangeTimelineCallback;
			//Bind함수
			UFUNCTION()
			void DFRangeTimelineUpdate(float Value);

		//Foliage
		UPROPERTY(EditAnywhere,  Category = "Timeline");
		class UCurveFloat* FoliageRangeCurve;
		//callback함수
		FOnTimelineFloat FoliageRangeTimelineCallback;
			//Bind함수
			UFUNCTION()
			void FoliageRangeTimelineUpdate(float Value);

	//DFStartRadius 타임라인
			//종료시
			FOnTimelineEvent DFStartRadiusTimelineFinishedCallback;
			UFUNCTION()
			void DFStartRadiusTimelineFinish();

			//Pull
			UPROPERTY(EditAnywhere, Category = "Timeline");
			class UCurveFloat* DFStartRadiusCurve;
			//callback함수
			FOnTimelineFloat DFStartRadiusTimelineCallback;
			//Bind함수
			UFUNCTION()
			void DFStartRadiusTimelineUpdate(float Value);

private:
	UFUNCTION()
	void OverlapPullRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapDFRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};

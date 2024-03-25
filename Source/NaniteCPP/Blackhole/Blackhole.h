
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

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BlackholeBaseMesh;

	UPROPERTY(EditAnywhere, Category = "ShpereCollision")
	class USphereComponent* TurnOffDFRange;
	UPROPERTY(EditAnywhere, Category = "ShpereCollision")
	class USphereComponent* PullRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UTimelineComponent* MeshTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UTimelineComponent* RangeTimeline;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//Die 함수
	bool DieToggle;
	UFUNCTION()
	void DieBlackhole();

	//Blackhole Mesh Size 타임 라인
		//MeshTimeline 종료시
		FOnTimelineEvent floatTimelineFinishedCallback;
			UFUNCTION()
			void SetScaleTimelineFinish();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"));
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
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"));
		class UCurveFloat* PullRangeCurve;
		//callback함수
		FOnTimelineFloat PullRangeTimelineCallback;
		//Bind함수
			UFUNCTION()
			void PullRangeTimelineUpdate(float Value);

		//DF
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"));
		class UCurveFloat* DFRangeCurve;
		//callback함수
		FOnTimelineFloat DFRangeTimelineCallback;
			//Bind함수
			UFUNCTION()
			void DFRangeTimelineUpdate(float Value);

private:
	UFUNCTION()
	void OverlapPullRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};

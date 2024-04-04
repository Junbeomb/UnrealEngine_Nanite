
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

	//Die �Լ�
	bool DieToggle;
	UFUNCTION()
	void DieBlackhole();

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
		UPROPERTY(EditAnywhere,  Category = "Timeline");
		class UCurveFloat* PullRangeCurve;
		//callback�Լ�
		FOnTimelineFloat PullRangeTimelineCallback;
		//Bind�Լ�
			UFUNCTION()
			void PullRangeTimelineUpdate(float Value);

		//DF
		UPROPERTY(EditAnywhere,  Category = "Timeline");
		class UCurveFloat* DFRangeCurve;
		//callback�Լ�
		FOnTimelineFloat DFRangeTimelineCallback;
			//Bind�Լ�
			UFUNCTION()
			void DFRangeTimelineUpdate(float Value);

		//Foliage
		UPROPERTY(EditAnywhere,  Category = "Timeline");
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

private:
	UFUNCTION()
	void OverlapPullRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapDFRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};

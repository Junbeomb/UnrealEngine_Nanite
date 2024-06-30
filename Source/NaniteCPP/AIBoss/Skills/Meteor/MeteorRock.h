// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "MeteorRock.generated.h"

UCLASS()
class NANITECPP_API AMeteorRock : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeteorMesh;

	UPROPERTY(EditDefaultsOnly)
	class AMeteorChargeCenter* Center;

	UFUNCTION()
	void AbsorbedByCenter();

	UMaterialInstanceDynamic* DMI;

	class UComp_BlendMesh* Comp_Blend;

	void MeteorRockBlendStart();
	void MeteorRockBlendFinish();
	bool IsBlendStart;

private://Timeline

	UTimelineComponent* DissapearTimeline;
	//Blackhole Mesh Size 타임 라인
		//MeshTimeline 종료시
	FOnTimelineEvent DissapearTimelineFinishedCallback;
	UFUNCTION()
	void DissapearTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	UCurveFloat* DissapearCurve;
	//callback함수
	FOnTimelineFloat DissapearTimelineCallback;
	//Bind함수
	UFUNCTION()
	void DissapearTimelineUpdate(float Value);
	
public:	
	AMeteorRock();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHomingBallBase> SoulBallChoice;

	UFUNCTION()
	void MeteorCenterBind();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PuzzleKeyDoor.generated.h"

UCLASS()
class NANITECPP_API APuzzleKeyDoor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	float saveZRotation;

private://Timeline
	UTimelineComponent* DoorTimeline;
	FOnTimelineEvent TimelineFinishedCallback;
	UFUNCTION() //함수 이름으로 Binding 하기 때문에, UFUNCTION() 키워드를 꼭 붙여야 리플렉션 시스템에 등록되어서 런타임 시에 함수를 찾기 가능.
	void SetDoorTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* DoorCurve;
	FOnTimelineFloat TimelineCallback;
	UFUNCTION() 
	void SetDoorTimelineUpdate(float Value);


protected:
	virtual void BeginPlay() override;

public:	
	APuzzleKeyDoor();
	void OpenDoor();

};

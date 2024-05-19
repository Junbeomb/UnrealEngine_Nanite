// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PuzzleSwitchDoor.generated.h"

DECLARE_MULTICAST_DELEGATE(FD_SwitchDoor);
UCLASS()
class NANITECPP_API APuzzleSwitchDoor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere)
	TArray<int> doorKeyNumArr;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<int> doorArr;

	UPROPERTY(EditAnywhere)
	class APuzzleSwitchBox* SwitchBox;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInstanceDynamic*> DMIList;

	//timeline=====
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* EmissiveTimeline;
	FOnTimelineEvent EmissiveTimelineFinishedCallback;
	UFUNCTION()
	void EmissiveTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* EmissiveCurve;
	//callback함수
	FOnTimelineFloat EmissiveTimelineCallback;
	//Bind함수
	UFUNCTION()
	void EmissiveTimelineUpdate(float Value);
	//timeline=====

	UFUNCTION()
	void TurnOn();

public:	
	APuzzleSwitchDoor();


	UFUNCTION()
	void pushElementDoorArr(int num);

	FD_SwitchDoor wrongAnswer;

protected:
	virtual void BeginPlay() override;

};

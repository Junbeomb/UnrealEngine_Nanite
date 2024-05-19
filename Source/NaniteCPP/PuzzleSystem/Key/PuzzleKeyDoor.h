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

	
	UPROPERTY()
	float saveZRotation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* DoorTimeline;

	FOnTimelineEvent TimelineFinishedCallback;
	UFUNCTION()
	void SetDoorTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* DoorCurve;
	//callback함수
	FOnTimelineFloat TimelineCallback;
	//Bind함수
	UFUNCTION()
	void SetDoorTimelineUpdate(float Value);

public:	
	// Sets default values for this actor's properties
	APuzzleKeyDoor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void OpenDoor();

};

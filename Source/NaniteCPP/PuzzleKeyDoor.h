// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "InteractionSystem/Interface_Interact.h"
#include "PuzzleKeyDoor.generated.h"

UCLASS()
class NANITECPP_API APuzzleKeyDoor : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere)
	class UComp_InteractBase* Comp_Interact;
	
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

	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OpenDoor();

};

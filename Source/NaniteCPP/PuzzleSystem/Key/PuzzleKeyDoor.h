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
	UFUNCTION() //�Լ� �̸����� Binding �ϱ� ������, UFUNCTION() Ű���带 �� �ٿ��� ���÷��� �ý��ۿ� ��ϵǾ ��Ÿ�� �ÿ� �Լ��� ã�� ����.
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:	
	APuzzleSwitchDoor();


	UFUNCTION()
	void pushElementDoorArr(int num);

	FD_SwitchDoor wrongAnswer;

protected:
	virtual void BeginPlay() override;

};

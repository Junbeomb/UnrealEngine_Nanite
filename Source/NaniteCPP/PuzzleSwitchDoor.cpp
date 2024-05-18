// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleSwitchDoor.h"


APuzzleSwitchDoor::APuzzleSwitchDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;
}

void APuzzleSwitchDoor::pushElementDoorArr(int num)
{
	doorArr.Add(num);

	//UE_LOG(LogTemp, Warning, TEXT("DoorArr.Num = %d ->(PuzzleSwitchDoor.cpp)"),doorArr.Num());
	//for (auto a : doorArr) {
	//	UE_LOG(LogTemp, Warning, TEXT("%d ->(PuzzleSwitchDoor.cpp)"),a);
	//}
	if (doorArr.Num() >= 4) {
		if (doorArr == doorKeyNumArr) {
			UE_LOG(LogTemp, Warning, TEXT("Answer!!->(PuzzleSwitchDoor.cpp)"));
			Destroy();
		}
		else {
			wrongAnswer.Broadcast();
			doorArr.Empty();
			UE_LOG(LogTemp, Warning, TEXT("Wrong!!->(PuzzleSwitchDoor.cpp)"));
		}
	}
}

void APuzzleSwitchDoor::BeginPlay()
{
	Super::BeginPlay();
	doorArr.Empty();
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleKeyDoor.h"

// Sets default values
APuzzleKeyDoor::APuzzleKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
	//callbackÇÔ¼öbind
	TimelineCallback.BindUFunction(this, FName("SetDoorTimelineUpdate"));
	TimelineFinishedCallback.BindUFunction(this, FName("SetDoorTimelineFinish"));
}

// Called when the game starts or when spawned
void APuzzleKeyDoor::BeginPlay()
{
	Super::BeginPlay();
	

	saveZRotation = GetActorRotation().Yaw;

}


void APuzzleKeyDoor::OpenDoor()
{
	if (DoorCurve) {
		DoorTimeline->AddInterpFloat(DoorCurve, TimelineCallback, FName("OpenDoorCurve"));
		DoorTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
		DoorTimeline->PlayFromStart();
	}
}

void APuzzleKeyDoor::SetDoorTimelineFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("FinishTimeline"));
}

void APuzzleKeyDoor::SetDoorTimelineUpdate(float Value)
{
	SetActorRotation({ 0,0,Value * -90 + saveZRotation });
}



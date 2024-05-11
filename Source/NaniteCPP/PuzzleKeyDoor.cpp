// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleKeyDoor.h"
#include "InteractionSystem/Comp_InteractBase.h"



// Sets default values
APuzzleKeyDoor::APuzzleKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	Comp_Interact = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_Interact"));
}

// Called when the game starts or when spawned
void APuzzleKeyDoor::BeginPlay()
{
	Super::BeginPlay();
	

	saveZRotation = GetActorRotation().Yaw;

}

// Called every frame
void APuzzleKeyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}

void APuzzleKeyDoor::SetDoorTimelineUpdate(float Value)
{
	SetActorRotation({ 0,0,Value * -90 + saveZRotation });
}

void APuzzleKeyDoor::PressEStart()
{
	OpenDoor();

}


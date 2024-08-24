
#include "PuzzleKeyDoor.h"

APuzzleKeyDoor::APuzzleKeyDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
	TimelineCallback.BindUFunction(this, FName("SetDoorTimelineUpdate"));
	TimelineFinishedCallback.BindUFunction(this, FName("SetDoorTimelineFinish"));
}

void APuzzleKeyDoor::BeginPlay()
{
	Super::BeginPlay();
	saveZRotation = GetActorRotation().Yaw;
}

void APuzzleKeyDoor::OpenDoor()
{
	if (DoorCurve) {
		//UE_LOG(LogTemp, Warning, TEXT("OpenDoor : DoorCuve"));
		DoorTimeline->AddInterpFloat(DoorCurve, TimelineCallback, FName("OpenDoorCurve"));
		DoorTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
		DoorTimeline->PlayFromStart();
	}
}

void APuzzleKeyDoor::SetDoorTimelineFinish()
{
	//UE_LOG(LogTemp, Warning, TEXT("FinishTimeline"));
}

void APuzzleKeyDoor::SetDoorTimelineUpdate(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Value);
	SetActorRotation({ 0,0,Value * -90 + saveZRotation });
}



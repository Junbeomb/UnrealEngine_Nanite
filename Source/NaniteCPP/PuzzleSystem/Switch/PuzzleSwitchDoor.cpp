
#include "PuzzleSwitchDoor.h"
#include "PuzzleSwitchBox.h"
#include "Kismet/GameplayStatics.h"


APuzzleSwitchDoor::APuzzleSwitchDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	//Timeline
	EmissiveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EmissiveTimeline"));
	EmissiveTimelineCallback.BindUFunction(this, FName("EmissiveTimelineUpdate"));
	EmissiveTimelineFinishedCallback.BindUFunction(this, FName("EmissiveTimelineFinish"));
}



void APuzzleSwitchDoor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < DoorMesh->GetNumMaterials(); i++) {
		UMaterialInstanceDynamic* TempDMI = DoorMesh->CreateDynamicMaterialInstance(i, DoorMesh->GetMaterial(i));
		DMIList.Add(TempDMI);
	}

	doorArr.Empty();

	SwitchBox = Cast<APuzzleSwitchBox>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleSwitchBox::StaticClass()));
	SwitchBox->SwitchBoxOn.AddUObject(this, &APuzzleSwitchDoor::TurnOn);
}

void APuzzleSwitchDoor::TurnOn()
{
	if (EmissiveCurve) {
		EmissiveTimeline->AddInterpFloat(EmissiveCurve, EmissiveTimelineCallback, FName("EmissiveCurve"));
		EmissiveTimeline->SetTimelineFinishedFunc(EmissiveTimelineFinishedCallback);
		EmissiveTimeline->PlayFromStart();
	}
}

void APuzzleSwitchDoor::EmissiveTimelineFinish()
{
}

void APuzzleSwitchDoor::EmissiveTimelineUpdate(float Value)
{
	FLinearColor colorF = { 0.5f,0.05f,0.05f,1.f };

	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetVectorParameterValue(TEXT("EmissiveColor"), Value * colorF);
	}
}

void APuzzleSwitchDoor::pushElementDoorArr(int num)
{
	doorArr.Add(num);

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



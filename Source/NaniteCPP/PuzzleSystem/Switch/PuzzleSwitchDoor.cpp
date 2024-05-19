// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleSwitchDoor.h"
#include "PuzzleSwitchBox.h"
#include "Kismet/GameplayStatics.h"


APuzzleSwitchDoor::APuzzleSwitchDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;
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
	FLinearColor colorF = { 0.5f,0.05f,0.05f,1.f };

	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetVectorParameterValue(TEXT("EmissiveColor"), colorF);
	}
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



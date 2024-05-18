#include "PuzzleSwitchButton.h"
#include "InteractionSystem/Comp_InteractBase.h"
#include "PuzzleSwitchDoor.h"
#include "Kismet/GameplayStatics.h"



APuzzleSwitchButton::APuzzleSwitchButton()
{
	PrimaryActorTick.bCanEverTick = false;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	RootComponent = ButtonMesh;

	Comp_Interact = CreateDefaultSubobject < UComp_InteractBase>(TEXT("Comp_Interact"));

}


void APuzzleSwitchButton::BeginPlay()
{
	Super::BeginPlay();
	SwitchDoor = Cast<APuzzleSwitchDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleSwitchDoor::StaticClass()));
	SwitchDoor->wrongAnswer.AddUObject(this, &APuzzleSwitchButton::ResetButton);

}

void APuzzleSwitchButton::ResetButton()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetButton!->(PuzzleSwitchButton.cpp)"));
	if (Comp_Interact == nullptr) {
		Comp_Interact = NewObject<UComp_InteractBase>(this, UComp_InteractBase::StaticClass(), NAME_None, RF_Transient);
		if (Comp_Interact) {
			Comp_Interact->RegisterComponent();
		}
	}
}

void APuzzleSwitchButton::PressEStart()
{
	if (Comp_Interact) {
		UE_LOG(LogTemp, Warning, TEXT("PressEStart!->(PuzzleSwitchButton.cpp)"));
		Comp_Interact->DestroyComponent();
		Comp_Interact = nullptr;
	}
		
	SwitchDoor->pushElementDoorArr(ButtonNum);
}



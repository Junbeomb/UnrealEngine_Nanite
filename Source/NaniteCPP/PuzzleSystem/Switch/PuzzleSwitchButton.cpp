#include "PuzzleSwitchButton.h"
#include "../../InteractionSystem/Comp_InteractBase.h"
#include "PuzzleSwitchDoor.h"
#include "PuzzleSwitchBox.h"
#include "Kismet/GameplayStatics.h"



APuzzleSwitchButton::APuzzleSwitchButton()
{
	PrimaryActorTick.bCanEverTick = false;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	RootComponent = ButtonMesh;

	Comp_Interact = CreateDefaultSubobject < UComp_InteractBase>(TEXT("Comp_Interact"));


	//Timeline
	EmissiveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EmissiveTimeline"));
	//callbackÇÔ¼öbind
	EmissiveTimelineCallback.BindUFunction(this, FName("EmissiveTimelineUpdate"));
	EmissiveTimelineFinishedCallback.BindUFunction(this, FName("EmissiveTimelineFinish"));
}


void APuzzleSwitchButton::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < ButtonMesh->GetNumMaterials(); i++) {
		UMaterialInstanceDynamic* TempDMI = ButtonMesh->CreateDynamicMaterialInstance(i, ButtonMesh->GetMaterial(i));
		DMIList.Add(TempDMI);
	}

	SwitchBox = Cast<APuzzleSwitchBox>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleSwitchBox::StaticClass()));
	SwitchBox->SwitchBoxOn.AddUObject(this, &APuzzleSwitchButton::TurnOn);

	SwitchDoor = Cast<APuzzleSwitchDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleSwitchDoor::StaticClass()));
	SwitchDoor->wrongAnswer.AddUObject(this, &APuzzleSwitchButton::ResetButton);

	
}

void APuzzleSwitchButton::TurnOn()
{

	if (EmissiveCurve) {
		EmissiveTimeline->AddInterpFloat(EmissiveCurve, EmissiveTimelineCallback, FName("EmissiveCurve"));
		EmissiveTimeline->SetTimelineFinishedFunc(EmissiveTimelineFinishedCallback);
		EmissiveTimeline->PlayFromStart();
	}
}

void APuzzleSwitchButton::EmissiveTimelineFinish()
{
}

void APuzzleSwitchButton::EmissiveTimelineUpdate(float Value)
{
	FLinearColor colorF = { 0.05f,0.05f,0.5f,1.f };

	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetVectorParameterValue(TEXT("EmissiveColor"), Value * colorF);
	}
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



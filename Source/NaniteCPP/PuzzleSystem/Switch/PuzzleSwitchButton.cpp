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
	SwitchDoor->wrongAnswer.AddUObject(this, &APuzzleSwitchButton::WrongFunc);

	
}

void APuzzleSwitchButton::TurnOn()
{
	if (EmissiveCurve) {
		EmissiveTimeline->AddInterpFloat(EmissiveCurve, EmissiveTimelineCallback, FName("EmissiveCurve"));
		EmissiveTimeline->SetTimelineFinishedFunc(EmissiveTimelineFinishedCallback);
		EmissiveTimeline->PlayFromStart();
	}
}

void APuzzleSwitchButton::WrongFunc()
{
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetVectorParameterValue(TEXT("EmissiveColor"), colorRed);
	}

	//delay
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			ResetButton();
		});

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, 1.5, false);

}

void APuzzleSwitchButton::EmissiveTimelineFinish()
{
	ResetButton();
}

void APuzzleSwitchButton::EmissiveTimelineUpdate(float Value)
{
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetVectorParameterValue(TEXT("EmissiveColor"), Value * colorDefault);
	}
}

void APuzzleSwitchButton::ResetButton()
{
	//UE_LOG(LogTemp, Warning, TEXT("ResetButton!->(PuzzleSwitchButton.cpp)"));

	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetVectorParameterValue(TEXT("EmissiveColor"), colorDefault);
	}
	if (Comp_Interact == nullptr) {
		Comp_Interact = NewObject<UComp_InteractBase>(this, UComp_InteractBase::StaticClass(), NAME_None, RF_Transient);
		if (Comp_Interact) {
			Comp_Interact->RegisterComponent();
		}
	}
}



void APuzzleSwitchButton::PressEStart()
{

	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetVectorParameterValue(TEXT("EmissiveColor"), colorBright);
	}

	if (Comp_Interact) {
		UE_LOG(LogTemp, Warning, TEXT("PressEStart!->(PuzzleSwitchButton.cpp)"));
		Comp_Interact->DestroyComponent();
		Comp_Interact = nullptr;
	}
		
	SwitchDoor->pushElementDoorArr(ButtonNum);
}



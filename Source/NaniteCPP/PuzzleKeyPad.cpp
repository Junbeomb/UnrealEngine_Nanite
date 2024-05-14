// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleKeyPad.h"
#include "Blending/Comp_BlendMesh.h"
#include "PuzzleKeyDoor.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionSystem/Comp_InteractBase.h"

// Sets default values
APuzzleKeyPad::APuzzleKeyPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	KeyPadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyPadMesh"));
	RootComponent = KeyPadMesh;

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

	isGetKey = false;

}

// Called when the game starts or when spawned
void APuzzleKeyPad::BeginPlay()
{
	Super::BeginPlay();

	Comp_Blend->D_FinishBlending.BindUObject(this, &APuzzleKeyPad::OnFinishBlending);

	
}

void APuzzleKeyPad::OnFinishBlending()
{
	//interact component Ãß°¡
	//Comp_Interact = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_Interact"));
	//Comp_Interact = Cast<UComp_InteractBase>(AddComponentByClass(UComp_InteractBase::StaticClass(), false, FTransform::Identity, false));
	Comp_Interact = NewObject<UComp_InteractBase>(this, UComp_InteractBase::StaticClass(), NAME_None, RF_Transient);
	if (Comp_Interact) {
		//UE_LOG(LogTemp, Warning, TEXT("FinishBlending PuzzleKeyPad"));
		Comp_Interact->RegisterComponent();
		//Comp_Interact->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		if (!isGetKey) {
			Comp_Interact->SetCantToggle(true);
		}
		else {
			Comp_Interact->SetCantToggle(false);
		}
	}
}

void APuzzleKeyPad::PressEStart()
{
	UE_LOG(LogTemp, Warning, TEXT("PressEStart PuzzleKeyPad"));

	APuzzleKeyDoor* puzzleDoor = Cast<APuzzleKeyDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleKeyDoor::StaticClass()));
	puzzleDoor->OpenDoor();

	Comp_Interact->DestroyThisComponentFunc();
}

void APuzzleKeyPad::GainKey()
{
	UE_LOG(LogTemp, Warning, TEXT("GainKey() PuzzleKeyPad"));
	isGetKey = true;

	if (IsValid(Comp_Interact)) {
		Comp_Interact->SetCantToggle(false);
	}
}

// Called every frame
void APuzzleKeyPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


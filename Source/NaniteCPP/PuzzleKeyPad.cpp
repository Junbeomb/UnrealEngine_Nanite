// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleKeyPad.h"
#include "Blending/Comp_BlendMesh.h"
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
	//interact component �߰�
	Comp_Interact = Cast<UComp_InteractBase>(AddComponentByClass(UComp_InteractBase::StaticClass(), false, FTransform::Identity, false));
	if (Comp_Interact) {
		Comp_Interact->RegisterComponent();
	}
}

void APuzzleKeyPad::PressEStart()
{
	UE_LOG(LogTemp, Warning, TEXT("PressEStart"));
	Destroy();
}

void APuzzleKeyPad::GetKey()
{
	isGetKey = true;
}

// Called every frame
void APuzzleKeyPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleKey.h"
#include "Blending/Comp_BlendMesh.h"
#include "InteractionSystem/Comp_InteractBase.h"



// Sets default values
APuzzleKey::APuzzleKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyStaticMesh"));
	RootComponent = KeyStaticMesh;
	
	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

}

// Called when the game starts or when spawned
void APuzzleKey::BeginPlay()
{
	Super::BeginPlay();

	Comp_Blend->D_FinishBlending.BindUObject(this, &APuzzleKey::OnFinishBlending);

	//Comp_Blend->D_FinishBlending.Execute();

}

void APuzzleKey::OnFinishBlending()
{
	//interact component Ãß°¡
	Comp_Interact =Cast<UComp_InteractBase>(AddComponentByClass(UComp_InteractBase::StaticClass(), false, FTransform::Identity, false));
	if (Comp_Interact) {
		Comp_Interact->RegisterComponent();
	}
}

void APuzzleKey::PressEStart()
{
	UE_LOG(LogTemp, Warning, TEXT("PressEStart"));
}

// Called every frame
void APuzzleKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


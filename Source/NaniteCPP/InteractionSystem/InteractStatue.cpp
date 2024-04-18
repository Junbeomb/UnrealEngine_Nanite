// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractStatue.h"

// Sets default values
AInteractStatue::AInteractStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	CompBase = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_InteractBase"));
}

void AInteractStatue::PressEStart()
{
	UE_LOG(LogTemp, Warning, TEXT("PressEStart"));
}

// Called when the game starts or when spawned
void AInteractStatue::BeginPlay()
{
	Super::BeginPlay();
	
	CompBase->SetDynamicMaterial(StaticMesh, false);

	UE_LOG(LogTemp, Warning, TEXT("asdf"));
}

// Called every frame
void AInteractStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


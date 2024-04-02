// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActorBase.h"

// Sets default values
AInteractActorBase::AInteractActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	CompBase = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_InteractBase"));
}

// Called when the game starts or when spawned
void AInteractActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	CompBase->SetDynamicMaterial(StaticMesh, false);

	UE_LOG(LogTemp, Warning, TEXT("asdf"));
}

// Called every frame
void AInteractActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


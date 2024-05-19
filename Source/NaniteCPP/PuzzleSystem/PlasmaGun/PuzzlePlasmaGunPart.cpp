// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlasmaGunPart.h"
#include "../../InteractionSystem/Comp_InteractBase.h"
#include "PuzzlePlasmaGunTable.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APuzzlePlasmaGunPart::APuzzlePlasmaGunPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartMesh"));
	RootComponent = PartMesh;

	Comp_Interact = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_Interact"));
}



// Called when the game starts or when spawned
void APuzzlePlasmaGunPart::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzlePlasmaGunPart::PressEStart()
{
	APuzzlePlasmaGunTable* table = Cast<APuzzlePlasmaGunTable>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzlePlasmaGunTable::StaticClass()));
	table->AddGunPart(PartNum);

	Destroy();
}


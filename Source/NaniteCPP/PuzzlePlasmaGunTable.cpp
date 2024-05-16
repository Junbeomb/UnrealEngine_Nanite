// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlasmaGunTable.h"
#include "InteractionSystem/Comp_InteractBase.h"
#include "PuzzlePlasmaGun.h"

// Sets default values
APuzzlePlasmaGunTable::APuzzlePlasmaGunTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	RootComponent = TableMesh;

}

void APuzzlePlasmaGunTable::AddGunPart(int num)
{
	GunPartArr.Add(num);

	if (GunPartArr.Num() >= 3) {

		Comp_Interact = NewObject<UComp_InteractBase>(this, UComp_InteractBase::StaticClass(), NAME_None, RF_Transient);
		if (Comp_Interact) {
			Comp_Interact->RegisterComponent();
		}

	}
}

void APuzzlePlasmaGunTable::PressEStart()
{
	//GunChildActor->SetChildActorClass(APuzzlePlasmaGun::StaticClass());
	//Comp_Interact->DestroyComponent();
}

// Called when the game starts or when spawned
void APuzzlePlasmaGunTable::BeginPlay()
{
	Super::BeginPlay();
	
}



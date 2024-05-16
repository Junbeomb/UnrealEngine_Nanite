
#include "PuzzlePlasmaGunTable.h"
#include "InteractionSystem/Comp_InteractBase.h"
#include "PuzzlePlasmaGun.h"

APuzzlePlasmaGunTable::APuzzlePlasmaGunTable()
{
	PrimaryActorTick.bCanEverTick = false;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	RootComponent = TableMesh;

	GunChildActor = CreateDefaultSubobject< UChildActorComponent>(TEXT("ChildActorGun"));
	GunChildActor->SetupAttachment(TableMesh);

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
	if (childActorClass) {
		GunChildActor->SetChildActorClass(childActorClass);
		Comp_Interact->DestroyComponent();
	}
	else {
		UE_LOG(LogTemp,Warning,TEXT("childActorClass not exist->(PuzzlePlasmaGunTable.cpp)"))
	}
}

// Called when the game starts or when spawned
void APuzzlePlasmaGunTable::BeginPlay()
{
	Super::BeginPlay();
}



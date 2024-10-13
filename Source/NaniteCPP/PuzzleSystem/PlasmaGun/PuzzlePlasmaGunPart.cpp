
#include "PuzzlePlasmaGunPart.h"
#include "../../InteractionSystem/Comp_InteractBase.h"
#include "PuzzlePlasmaGunTable.h"
#include "Kismet/GameplayStatics.h"


APuzzlePlasmaGunPart::APuzzlePlasmaGunPart()
{
	PrimaryActorTick.bCanEverTick = false;

	PartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartMesh"));
	RootComponent = PartMesh;

	Comp_Interact = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_Interact"));
}


void APuzzlePlasmaGunPart::PressEStart()
{
	APuzzlePlasmaGunTable* table = Cast<APuzzlePlasmaGunTable>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzlePlasmaGunTable::StaticClass()));
	table->AddGunPart(PartNum);

	Destroy();
}


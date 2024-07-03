#include "FinalChargeCenter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


AFinalChargeCenter::AFinalChargeCenter()
{
	PrimaryActorTick.bCanEverTick = true;

	CenterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterMesh"));
	RootComponent = CenterMesh;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

}


void AFinalChargeCenter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFinalChargeCenter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


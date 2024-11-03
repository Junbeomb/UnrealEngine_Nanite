
#include "MaterialChangeBall.h"

AMaterialChangeBall::AMaterialChangeBall()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChangeBallMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCastShadow(false);

	CountTick = 0.f;
}

void AMaterialChangeBall::BeginPlay()
{
	Super::BeginPlay();
	
	InitialScale = StaticMesh->GetRelativeScale3D();
	
	StaticMesh->SetWorldScale3D({ 0,0,0 });
}

void AMaterialChangeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CountTick += DeltaTime;
	if (CountTick < 1.0) {
		StaticMesh->SetWorldScale3D({ CountTick,CountTick,CountTick });	
	}
	else {
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}


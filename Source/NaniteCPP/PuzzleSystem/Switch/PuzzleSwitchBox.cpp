
#include "PuzzleSwitchBox.h"
#include "../../Blending/Comp_BlendMesh.h"

APuzzleSwitchBox::APuzzleSwitchBox()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box_Mesh"));
	RootComponent = BoxMesh;

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

}

void APuzzleSwitchBox::BeginPlay()
{
	Super::BeginPlay();

	Comp_Blend->D_FinishBlending.BindUObject(this, &APuzzleSwitchBox::OnFinishBlending);
}

void APuzzleSwitchBox::OnFinishBlending()
{
	SwitchBoxOn.Broadcast();
}


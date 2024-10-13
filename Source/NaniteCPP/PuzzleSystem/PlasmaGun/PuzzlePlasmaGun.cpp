
#include "PuzzlePlasmaGun.h"

APuzzlePlasmaGun::APuzzlePlasmaGun()
{
	PrimaryActorTick.bCanEverTick = false;

	GunSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunSkeletalMesh"));
	RootComponent = GunSkeletalMesh;

}

void APuzzlePlasmaGun::PressEStart()
{
	Destroy();
}



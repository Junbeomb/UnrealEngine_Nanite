// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlasmaGun.h"

// Sets default values
APuzzlePlasmaGun::APuzzlePlasmaGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GunSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunSkeletalMesh"));
	RootComponent = GunSkeletalMesh;

}

void APuzzlePlasmaGun::PressEStart()
{
	Destroy();
}

// Called when the game starts or when spawned
void APuzzlePlasmaGun::BeginPlay()
{
	Super::BeginPlay();
	
}



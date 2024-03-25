// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageHelpProjectile.h"

// Sets default values
AFoliageHelpProjectile::AFoliageHelpProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoliageHelpProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("FoliageHelpProjectil Spawn!!!!"));
}

// Called every frame
void AFoliageHelpProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


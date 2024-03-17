// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliagePlantBase.h"

// Sets default values
AFoliagePlantBase::AFoliagePlantBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoliagePlantBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoliagePlantBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


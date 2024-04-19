// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialChangeBall.h"

// Sets default values
AMaterialChangeBall::AMaterialChangeBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChangeBallMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AMaterialChangeBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaterialChangeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


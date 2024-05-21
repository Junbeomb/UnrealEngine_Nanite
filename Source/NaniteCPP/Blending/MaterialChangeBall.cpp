// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialChangeBall.h"

// Sets default values
AMaterialChangeBall::AMaterialChangeBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChangeBallMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCastShadow(false);
	CountTick = 0.f;
}

// Called when the game starts or when spawned
void AMaterialChangeBall::BeginPlay()
{
	Super::BeginPlay();
	
	InitialScale = StaticMesh->GetRelativeScale3D();
	
	StaticMesh->SetWorldScale3D({ 0,0,0 });
	PrimaryActorTick.SetTickFunctionEnable(true);
}

// Called every frame
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

	UE_LOG(LogTemp, Warning, TEXT("%f"),CountTick);
}


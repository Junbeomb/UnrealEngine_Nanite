// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHammer.h"

// Sets default values
ABossHammer::ABossHammer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HammerStatic = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HammerStatic"));
	RootComponent = HammerStatic;
}

// Called when the game starts or when spawned
void ABossHammer::BeginPlay()
{
	Super::BeginPlay();
	
}



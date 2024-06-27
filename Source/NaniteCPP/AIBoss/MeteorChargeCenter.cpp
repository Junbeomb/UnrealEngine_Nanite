// Fill out your copyright notice in the Description page of Project Settings.


#include "MeteorChargeCenter.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AMeteorChargeCenter::AMeteorChargeCenter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;*/

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	RootComponent = Niagara;
	//Niagara->SetupAttachment(SphereCollision);
}

void AMeteorChargeCenter::CallDAbsortStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("CallDAbsortStart"));
	D_AbsortStart.Broadcast();
}

// Called when the game starts or when spawned
void AMeteorChargeCenter::BeginPlay()
{
	Super::BeginPlay();
	
}



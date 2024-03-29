// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeLightBase.h"

// Sets default values
ABlackholeLightBase::ABlackholeLightBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	BaseStaticMesh->SetupAttachment(RootComponent);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(BaseStaticMesh);

	BlackholeComp = CreateDefaultSubobject<UBlackholeCompBase>(TEXT("BlackholeComp"));

}

// Called when the game starts or when spawned
void ABlackholeLightBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackholeLightBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


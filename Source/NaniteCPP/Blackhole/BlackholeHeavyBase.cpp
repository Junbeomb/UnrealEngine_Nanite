// Fill out your copyright notice in the Description page of Project Settings.



#include "BlackholeHeavyBase.h"


// Sets default values
ABlackholeHeavyBase::ABlackholeHeavyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	BaseStaticMesh->SetupAttachment(RootComponent);
		
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(BaseStaticMesh);

	BlackholeComp = CreateDefaultSubobject<UBlackholeCompBase>(TEXT("BlackholeComp"));

	RotationStrength = 10.f;
}

// Called when the game starts or when spawned
void ABlackholeHeavyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackholeHeavyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BlackholeComp->GetIsPull()) {
		BaseStaticMesh->AddLocalRotation( BlackholeComp->GetInitialRotationDegree()* RotationStrength* DeltaTime);
	}

}


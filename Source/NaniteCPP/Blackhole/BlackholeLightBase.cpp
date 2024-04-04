// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeLightBase.h"

// Sets default values
ABlackholeLightBase::ABlackholeLightBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	RootComponent = BaseStaticMesh;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(BaseStaticMesh);

	BlackholeComp = CreateDefaultSubobject<UBlackholeCompBase>(TEXT("BlackholeComp"));

	RotationStrength = 20.f;

}

// Called when the game starts or when spawned
void ABlackholeLightBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackholeLightBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Niagara->SetVisibility(false);
	Niagara->SetHiddenInGame(true);
	Niagara->SetAutoActivate(false);
}
// Called every frame
void ABlackholeLightBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BlackholeComp->GetIsPull()) {
		BaseStaticMesh->AddLocalRotation(BlackholeComp->GetInitialRotationDegree() * RotationStrength * DeltaTime);
	}
}


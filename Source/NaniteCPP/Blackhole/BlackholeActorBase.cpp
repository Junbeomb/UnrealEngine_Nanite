
#include "BlackholeActorBase.h"
#include "BlackholeCompBase.h"
#include "NiagaraFunctionLibrary.h"

ABlackholeActorBase::ABlackholeActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	RootComponent = BaseStaticMesh;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(BaseStaticMesh);

	BlackholeComp = CreateDefaultSubobject<UBlackholeCompBase>(TEXT("BlackholeComp"));

	RotationStrength = 10.f;
}

// Called when the game starts or when spawned
void ABlackholeActorBase::BeginPlay() 
{
	Super::BeginPlay();
}

void ABlackholeActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Niagara->SetVisibility(false);
	Niagara->SetHiddenInGame(true);
	Niagara->SetAutoActivate(false);
}

// Called every frame
void ABlackholeActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BlackholeComp->GetIsPull()) {
		BaseStaticMesh->AddLocalRotation(BlackholeComp->GetInitialRotationDegree() * RotationStrength * DeltaTime);
	}
}


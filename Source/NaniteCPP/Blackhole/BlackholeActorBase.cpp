
#include "BlackholeActorBase.h"
#include "BlackholeCompBase.h"
#include "NiagaraFunctionLibrary.h"

ABlackholeActorBase::ABlackholeActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	RootComponent = BaseStaticMesh;

	BaseStaticMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

	BlackholeActorNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	BlackholeActorNiagara->SetupAttachment(BaseStaticMesh);

	BlackholeComp = CreateDefaultSubobject<UBlackholeCompBase>(TEXT("BlackholeComp"));

	RotationStrength = 10.f;
}

void ABlackholeActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BlackholeActorNiagara->SetVisibility(false);
	BlackholeActorNiagara->SetHiddenInGame(true);
	BlackholeActorNiagara->SetAutoActivate(false);
}

void ABlackholeActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BlackholeComp->GetIsPull()) {
		BaseStaticMesh->AddLocalRotation(BlackholeComp->GetInitialRotationDegree() * RotationStrength * DeltaTime);
	}
}

void ABlackholeActorBase::SetRotationStrength(float value)
{
	RotationStrength = value;
}

UStaticMeshComponent* ABlackholeActorBase::GetBaseStaticMesh()
{
	return BaseStaticMesh;
}


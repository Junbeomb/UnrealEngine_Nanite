#include "BlendingProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Comp_BlendMesh.h"
#include "MaterialChangeBall.h"

ABlendingProjectile::ABlendingProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	FoliageChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("FoliageChildActor"));
	FoliageChildActor->SetupAttachment(RootComponent);

	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

}

void ABlendingProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetCollisionComp()->OnComponentHit.AddDynamic(this, &ABlendingProjectile::OnHit);
}

void ABlendingProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UComp_BlendMesh* UBM = Cast<UComp_BlendMesh>(OtherActor->GetComponentByClass(UComp_BlendMesh::StaticClass()));

	if (IsValid(UBM)) {
		if (!UBM->IsBlendStart && ChangeBall) {
			GetWorld()->SpawnActor<AMaterialChangeBall>(ChangeBall, GetActorTransform(), ActorSpawnParams);
			UBM->StartBlend();
		}
	}

	Destroy();
	return;
}
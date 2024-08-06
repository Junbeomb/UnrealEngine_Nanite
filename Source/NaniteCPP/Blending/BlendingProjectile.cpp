#include "BlendingProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Comp_BlendMesh.h"
#include "MaterialChangeBall.h"


// Sets default values
ABlendingProjectile::ABlendingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	RootComponent = CollisionComp;
	CollisionComp->OnComponentHit.AddDynamic(this, &ABlendingProjectile::OnHit);

	BasicProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlendingBaseMesh"));
	BasicProjectileMesh->SetupAttachment(RootComponent);

	FoliageChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("FoliageChildActor"));
	FoliageChildActor->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

}

void ABlendingProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("Destroy hit"));

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
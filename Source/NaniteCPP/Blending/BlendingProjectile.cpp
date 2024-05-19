// Fill out your copyright notice in the Description page of Project Settings.


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

	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	RootComponent = HitSphere;

	BlendingBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlendingBaseMesh"));
	BlendingBaseMesh->SetupAttachment(RootComponent);

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

void ABlendingProjectile::HitProjectile(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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


// Called when the game starts or when spawned
void ABlendingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	HitSphere->OnComponentHit.AddDynamic(this, &ABlendingProjectile::HitProjectile);
}

// Called every frame
void ABlendingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


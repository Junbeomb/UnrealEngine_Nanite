// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BlendingProjectile.h"

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

}

// Called when the game starts or when spawned
void ABlendingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlendingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"

#include "Blackhole.h"

// Sets default values
ABlackholeProjectile::ABlackholeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(25.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABlackholeProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	BasicProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeProjectileMesh"));
	BasicProjectileMesh->SetupAttachment(RootComponent);

	//�޽� ����
	//ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/1_Blackhole/SM_BlackholeMesh"));
	//BasicProjectileMesh->SetStaticMesh(MeshAsset.Object);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void ABlackholeProjectile::OnHit(UPrimitiveComponent* HitComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, 
	const FHitResult& Hit)
{

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//�׳� Blackhole.h �� �ҷ��ְ� �Ǹ� FoliageInfluencer �� null�� ����Ȧ�� ��ȯ�ȴ�.(ChildActorClass�� ������ BP_Blackhole�� �ҷ��־�� �Ѵ�.)
		if (BPBlackhole) {
			//UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
			GetWorld()->SpawnActor<ABlackhole>(BPBlackhole, GetActorLocation(), {0,0,0}, ActorSpawnParams);
		}
		Destroy();
	}
}

#include "BlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"

#include "Blackhole.h"

ABlackholeProjectile::ABlackholeProjectile()
{

}

void ABlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetCollisionComp()->OnComponentHit.AddDynamic(this, &ABlackholeProjectile::OnHit);
}

void ABlackholeProjectile::OnHit(UPrimitiveComponent* HitComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, 
	const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)){
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (BPBlackhole) {
			GetWorld()->SpawnActor<ABlackhole>(BPBlackhole, GetActorLocation(), {0,0,0}, ActorSpawnParams);
		}
		Destroy();
	}
}
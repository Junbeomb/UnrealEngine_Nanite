// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"

#include "Blackhole.h"

ABlackholeProjectile::ABlackholeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic(this, &ABlackholeProjectile::OnHit);
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

		//그냥 Blackhole.h 를 불러주게 되면 FoliageInfluencer 가 null인 블랙홀이 소환된다.(ChildActorClass가 설정된 BP_Blackhole을 불러주어야 한다.)
		if (BPBlackhole) {
			//UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
			GetWorld()->SpawnActor<ABlackhole>(BPBlackhole, GetActorLocation(), {0,0,0}, ActorSpawnParams);
		}
		Destroy();
	}
}
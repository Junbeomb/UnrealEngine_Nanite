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

		//�׳� Blackhole.h �� �ҷ��ְ� �Ǹ� FoliageInfluencer �� null�� ��Ȧ�� ��ȯ�ȴ�.(ChildActorClass�� ������ BP_Blackhole�� �ҷ��־�� �Ѵ�.)
		if (BPBlackhole) {
			//UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
			GetWorld()->SpawnActor<ABlackhole>(BPBlackhole, GetActorLocation(), {0,0,0}, ActorSpawnParams);
		}
		Destroy();
	}
}
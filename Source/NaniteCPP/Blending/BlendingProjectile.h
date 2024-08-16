// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../BaseProjectile.h"

#include "BlendingProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class NANITECPP_API ABlendingProjectile : public ABaseProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Blending)
	UChildActorComponent* FoliageChildActor;

	UPROPERTY(EditAnywhere, Category = Blending)
	TSubclassOf<class AMaterialChangeBall> ChangeBall;

	FActorSpawnParameters ActorSpawnParams;

public:	
	ABlendingProjectile();

protected:
	void BeginPlay() override;

	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override ;
};

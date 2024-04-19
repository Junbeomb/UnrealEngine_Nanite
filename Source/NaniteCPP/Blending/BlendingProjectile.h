// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlendingProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class NANITECPP_API ABlendingProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category= Blending)
	UStaticMeshComponent* BlendingBaseMesh;

	UPROPERTY(EditAnywhere, Category = Blending)
	UChildActorComponent* FoliageChildActor;

	UPROPERTY(EditAnywhere, Category = Blending)
	class USphereComponent* HitSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	ABlendingProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

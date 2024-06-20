// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossHomingBall.generated.h"

UCLASS()
class NANITECPP_API ABossHomingBall : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	ABossHomingBall();

	UPROPERTY(EditAnywhere)
	AActor* Target;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossHomingBall.generated.h"

DECLARE_MULTICAST_DELEGATE(FD_HitActor);
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
	
public:	
	// Sets default values for this actor's properties
	ABossHomingBall();

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	AActor* Target;

	FD_HitActor D_Hit;

	//블프에서 OnComponentHIt 노드에 Printstring 찍으면 실행이 되지만 c++ 만 쓰면 실행이 안돼 왜그럼
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

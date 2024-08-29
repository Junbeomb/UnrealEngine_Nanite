#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class NANITECPP_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent* BasicProjectileMesh;

	USphereComponent* CollisionComp;

	UProjectileMovementComponent* ProjectileMovement;

protected:

	USphereComponent* GetCollisionComp();
	
	UFUNCTION() //부모에 UFUNCTION() 붙이면 자식들은 안붙여도 리플렉션 시스템 사용가능
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {};

	virtual void BeginPlay() override;

public:
	ABaseProjectile();

};

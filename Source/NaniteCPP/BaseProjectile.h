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
	
	UFUNCTION() //�θ� UFUNCTION() ���̸� �ڽĵ��� �Ⱥٿ��� ���÷��� �ý��� ��밡��
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {};

	virtual void BeginPlay() override;

public:
	ABaseProjectile();

};

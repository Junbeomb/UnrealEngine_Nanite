
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "HomingBallBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FD_HitActor);
UCLASS()
class NANITECPP_API AHomingBallBase : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComp;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	FD_HitActor D_Hit;
	
public:	
	
	AHomingBallBase();

	UPROPERTY(EditAnywhere)
	AActor* TargetActor;

	//블프에서 OnComponentHIt 노드에 Printstring 찍으면 실행이 되지만 c++ 만 쓰면 실행이 안돼 왜그럼
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};

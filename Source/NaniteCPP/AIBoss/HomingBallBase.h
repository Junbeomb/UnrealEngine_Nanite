
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

	//�������� OnComponentHIt ��忡 Printstring ������ ������ ������ c++ �� ���� ������ �ȵ� �ֱ׷�
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};

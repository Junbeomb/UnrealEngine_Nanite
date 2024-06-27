
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingBallBase.generated.h"

UCLASS()
class NANITECPP_API AHomingBallBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	
	AHomingBallBase();

	UPROPERTY(EditAnywhere)
	AActor* TargetActor;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialChangeBall.generated.h"

UCLASS()
class NANITECPP_API AMaterialChangeBall : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	FVector InitialScale;

	float CountTick;

protected:
	virtual void BeginPlay() override;
public:	
	AMaterialChangeBall();
	virtual void Tick(float DeltaTime) override;


};


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalChargeCenter.generated.h"

UCLASS()
class NANITECPP_API AFinalChargeCenter : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CenterMesh;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;
	
public:	
	AFinalChargeCenter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

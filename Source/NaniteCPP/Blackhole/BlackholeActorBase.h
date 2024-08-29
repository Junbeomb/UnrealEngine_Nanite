#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackholeActorBase.generated.h"

UCLASS()
class NANITECPP_API ABlackholeActorBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* BlackholeActorNiagara;

	float RotationStrength;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* L_Niagara;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* H_Niagara;

protected:
	void OnConstruction(const FTransform& Transform) override;

	class UBlackholeCompBase* BlackholeComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseStaticMesh;

public:
	ABlackholeActorBase();
	void Tick(float DeltaTime) override;

	void SetRotationStrength(float);

	UStaticMeshComponent* GetBaseStaticMesh();




};


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../InteractionSystem/Interface_Interact.h"
#include "PuzzlePlasmaGunPart.generated.h"

UCLASS()
class NANITECPP_API APuzzlePlasmaGunPart : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PartMesh;

	class UComp_InteractBase* Comp_Interact;

	UPROPERTY(EditAnywhere)
	int PartNum;
	
public:	
	APuzzlePlasmaGunPart();

	virtual void PressEStart() override;


};

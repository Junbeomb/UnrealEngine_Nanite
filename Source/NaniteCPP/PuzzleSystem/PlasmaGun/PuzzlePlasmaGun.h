#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../InteractionSystem/Interface_Interact.h"
#include "PuzzlePlasmaGun.generated.h"

UCLASS()
class NANITECPP_API APuzzlePlasmaGun : public AActor, public IInterface_Interact
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* GunSkeletalMesh;

public:	
	APuzzlePlasmaGun();
	void PressEStart() override;

};

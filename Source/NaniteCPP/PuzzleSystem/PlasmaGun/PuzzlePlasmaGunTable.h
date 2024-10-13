
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../InteractionSystem/Interface_Interact.h"
#include "PuzzlePlasmaGunTable.generated.h"

UCLASS()
class NANITECPP_API APuzzlePlasmaGunTable : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TableMesh;
	
	UPROPERTY(EditAnywhere)
	TArray<int> GunPartArr;

	UPROPERTY(EditAnywhere)
	class UComp_InteractBase* Comp_Interact;

	UPROPERTY(EditAnywhere)
	UChildActorComponent* GunChildActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> childActorClass;

public:	
	APuzzlePlasmaGunTable();

	void AddGunPart(int num);

	virtual void PressEStart() override;



};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Comp_InteractBase.h"
#include "Interface_Interact.h"
#include "InteractActorBase.generated.h"


UCLASS()
class NANITECPP_API AInteractActorBase : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	AInteractActorBase();

	UPROPERTY(EditAnywhere)
	UComp_InteractBase* CompBase;

	virtual void PressEStart() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

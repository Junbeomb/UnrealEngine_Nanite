#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Comp_InteractBase.h"
#include "InteractActorBase.generated.h"


UCLASS()
class NANITECPP_API AInteractActorBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UComp_InteractBase* CompBase;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	AInteractActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

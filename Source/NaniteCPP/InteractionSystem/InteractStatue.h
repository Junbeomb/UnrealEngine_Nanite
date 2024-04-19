#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Comp_InteractBase.h"
#include "Interface_Interact.h"
#include "InteractStatue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FD_Delegate);
UCLASS()
class NANITECPP_API AInteractStatue : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	AInteractStatue();

	UPROPERTY(EditAnywhere)
	UComp_InteractBase* CompBase;

	virtual void PressEStart() override;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FD_Delegate D_BPStartBlend;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FD_Delegate D_BPFinishBlend;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere)
	class UComp_InteractBase* Comp_Interact;

	UPROPERTY(EditAnywhere)
	int PartNum;
	
public:	
	// Sets default values for this actor's properties
	APuzzlePlasmaGunPart();

	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

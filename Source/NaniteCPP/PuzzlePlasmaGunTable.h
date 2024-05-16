// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionSystem/Interface_Interact.h"
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
	// Sets default values for this actor's properties
	APuzzlePlasmaGunTable();

	UFUNCTION()
	void AddGunPart(int num);

	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};

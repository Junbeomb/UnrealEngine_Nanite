// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	APuzzlePlasmaGun();

	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

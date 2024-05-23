// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "BlackholeCompBase.h"
#include "BlackholeHeavyBase.generated.h"

UCLASS()
class NANITECPP_API ABlackholeHeavyBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere)
	UBlackholeCompBase* BlackholeComp;

	UPROPERTY(EditAnywhere)
	float RotationStrength;




public:	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseStaticMesh;
	// Sets default values for this actor's properties

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* L_Niagara;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* H_Niagara;

	ABlackholeHeavyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

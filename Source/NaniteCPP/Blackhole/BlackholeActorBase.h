// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackholeActorBase.generated.h"

UCLASS()
class NANITECPP_API ABlackholeActorBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere)
	class UBlackholeCompBase* BlackholeComp;

	UPROPERTY(EditAnywhere)
	float RotationStrength;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseStaticMesh;
	// Sets default values for this actor's properties

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* L_Niagara;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* H_Niagara;

	ABlackholeActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

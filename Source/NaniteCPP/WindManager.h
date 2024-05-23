// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindStructData.h"
#include "WindManager.generated.h"

UCLASS()
class NANITECPP_API AWindManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;
	
	UPROPERTY(EditAnywhere)
	int InputGridResolution;

	UPROPERTY(EditAnywhere)
	float SimulationSizeWS;


	UFUNCTION()
	void SetInitialVariable();
	UFUNCTION()
	void SetPlayerPawnLocation();
	UPROPERTY(EditAnywhere)
	USceneComponent* PlayerComponent;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* ParamCollection;
	class UMaterialParameterCollectionInstance* PCI;

	UPROPERTY(EditAnywhere)
	class UTextureRenderTarget2D* WindRenderTarget;

	UFUNCTION()
	void ResetTick();

	UPROPERTY(EditAnywhere)
	FVector PlayerLocation;

	UPROPERTY(EditAnywhere)
	TArray<FWINDDATA> SWindData;

	UPROPERTY(EditAnywhere)
	TArray<FVector4> WindStartLocationRadius;
	UPROPERTY(EditAnywhere)
	TArray<FVector4> WindStartVelocityStrength;

	UFUNCTION()
	void WindStructDataToTranslate();

	TArray<int> DeleteIndexList;

public:	
	// Sets default values for this actor's properties
	AWindManager();

	UFUNCTION()
	void ResetVariable();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

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
	
	float InputGridResolution;
	float SimulationSizeWS;


	void SetInitialVariable();
	void SetPlayerPawnLocation();
	UPROPERTY()
	USceneComponent* PlayerComponent;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* ParamCollection;
	class UMaterialParameterCollectionInstance* PCI;

	UPROPERTY(EditAnywhere)
	class UTextureRenderTarget2D* WindRenderTarget;

	void ResetTick();

	FVector PlayerLocation;

	TArray<FWINDDATA> SWindData;

	TArray<FVector4> WindStartLocationRadius;
	TArray<FVector4> WindStartVelocityStrength;

	void WindStructDataToTranslate();



	void CacluatePlayerPosition();
	void CacluateWindPosition();
	void SetGridVariable();

	FVector2D windLocation;

	FVector2D windOffset;
public:	

	AWindManager();

	virtual void Tick(float DeltaTime) override;
	void ResetVariable();

	void AddWindAtWindData(FWINDDATA wsd);
protected:

	virtual void BeginPlay() override;
};

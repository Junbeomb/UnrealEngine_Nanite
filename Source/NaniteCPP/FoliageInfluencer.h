// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoliageInfluencer.generated.h"

UCLASS()
class NANITECPP_API AFoliageInfluencer : public AActor
{
	GENERATED_BODY()


public:	
	AFoliageInfluencer();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Foliage")
	TArray<TSubclassOf<AActor>> FoliageBlueprints;

	//LineTrace Radius;
	UPROPERTY(EditAnywhere, Category = "Foliage")
	float PhysicsRadius;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* FoliageTransMesh;


	//ºÎµúÈùcomponent, actorµéÀ» Ã£´Â ÇÔ¼ö
	bool FindTrace();
	TArray<FHitResult> OutResults;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

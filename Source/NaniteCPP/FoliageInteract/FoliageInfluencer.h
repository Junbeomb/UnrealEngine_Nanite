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
	TArray<TSubclassOf<AActor>> FoliageBlueprints; //TSubclassOf : 월드내에 배치되어있는 actor 이외에도 다 가져올 수 있음.

	//LineTrace Radius;
	UPROPERTY(EditAnywhere, Category = "Foliage")
	float PhysicsRadius;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* FoliageTransMesh;


	//부딪힌component, actor들을 찾는 함수
	bool FindTrace();
	TArray<FHitResult> OutResults;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

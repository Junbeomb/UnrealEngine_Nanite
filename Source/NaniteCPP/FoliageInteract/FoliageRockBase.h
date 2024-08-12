// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FoliageBase.h"

#include "FoliageRockBase.generated.h"

UCLASS()
class NANITECPP_API AFoliageRockBase : public AFoliageBase
{
	GENERATED_BODY()
	
public:	
	AFoliageRockBase();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

protected:

	void SetCollisionSimulate() override;
	void GetMeshName(FString& LeftS, FString& RightS) override;
	void AddFoliageInstance(UActorComponent*) override;
	void GoCustomFunc() override;

};

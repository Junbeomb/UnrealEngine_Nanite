// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindBomb.generated.h"

class ABlackhole;
class AInteractStatue;
UCLASS()
class NANITECPP_API AWindBomb : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class AWindManager* windManager;

	UFUNCTION()
	void StartWind();

	UPROPERTY(EditAnywhere)
	float Duration;
	UPROPERTY(EditAnywhere)
	float Radius;
	UPROPERTY(EditAnywhere)
	float Strength;

	UPROPERTY(EditAnywhere)
	UCurveFloat* StrengthCurve;

public:	
	AWindBomb();


	void BlackholeFunc(ABlackhole& bh);

	void StatueFunc(AInteractStatue& is);
	



};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindBomb.generated.h"

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
	// Sets default values for this actor's properties
	AWindBomb();

	UFUNCTION()
	void BlackholeFunc();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

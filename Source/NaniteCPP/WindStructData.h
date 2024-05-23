// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindStructData.generated.h"

class NANITECPP_API WindStructData
{
public:
	WindStructData();
	~WindStructData();
};

USTRUCT(Atomic)
struct FWINDDATA {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Duration;
	UPROPERTY(EditAnywhere)
	float StartTime;
	UPROPERTY(EditAnywhere)
	UCurveFloat* StrengthCurve;
	UPROPERTY(EditAnywhere)
	FVector4 S_WindStartLocRad;
	UPROPERTY(EditAnywhere)
	FVector4 S_WindStartVelStr;
};

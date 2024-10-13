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
	float Duration;
	float StartTime;
	UCurveFloat* StrengthCurve;
	FVector4 S_WindStartLocRad;
	FVector4 S_WindStartVelStr;
};

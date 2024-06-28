// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingBallBase.h"
#include "MeteorSoulBall.generated.h"

UCLASS()
class NANITECPP_API AMeteorSoulBall : public AHomingBallBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingBallBase.h"
#include "BossHomingBall.generated.h"

UCLASS()
class NANITECPP_API ABossHomingBall : public AHomingBallBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

};

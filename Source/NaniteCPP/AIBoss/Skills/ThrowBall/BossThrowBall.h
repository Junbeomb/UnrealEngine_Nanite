// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../HomingBallBase.h"
#include "BossThrowBall.generated.h"

UCLASS()
class NANITECPP_API ABossThrowBall : public AHomingBallBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

};

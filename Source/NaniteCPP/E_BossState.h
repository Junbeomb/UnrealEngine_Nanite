// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class NANITECPP_API E_BossState
{
public:
	E_BossState();
	~E_BossState();
};

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attack UMETA(DisplayName = "Attack"),
	Frozen UMETA(DisplayName = "Frozen"),
	Death UMETA(DisplayName = "Death")
};

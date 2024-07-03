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

UENUM(BlueprintType)
enum class EBossSenseType : uint8
{
	None UMETA(DisplayName = "None"),
	Sight UMETA(DisplayName = "Sight"),
	Damage UMETA(DisplayName = "Damage")
};

UENUM(BlueprintType)
enum class EBossSkill : uint8
{
	Combo1 UMETA(DisplayName = "Combo1"),
	Jump UMETA(DisplayName = "Jump"),
	ThrowBall UMETA(DisplayName = "ThrowBall"),
	Meteor UMETA(DisplayName = "Meteor"),
	Final UMETA(DisplayName = "Final")

};

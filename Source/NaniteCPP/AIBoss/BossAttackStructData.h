// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "BossAttackStructData.generated.h"
/**
 * 
 */
class NANITECPP_API BossAttackStructData
{
public:
	BossAttackStructData();
	~BossAttackStructData();
};

USTRUCT(Atomic)
struct FBOSSATTACKDATA {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	AActor* AttackTarget;
	UPROPERTY(EditAnywhere)
	float DamageAmount;
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "E_BossState.h"
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
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	EBossSkill CurrentSkill;
	UPROPERTY(EditAnywhere)
	float DamageAmount;
	UPROPERTY(EditAnywhere)
	float radius;
	UPROPERTY(EditAnywhere)
	float length;


	
};
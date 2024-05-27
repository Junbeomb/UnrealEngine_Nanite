// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "E_BossState.h"


#include "AIC_BossBase.generated.h"

/**
 * 
 */
UCLASS()
class NANITECPP_API AAIC_BossBase : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class AAIBossBase* BossBase;

	UPROPERTY(EditAnywhere)
	EBossState BState;

	UFUNCTION()
	void SetStateAsPassive();

	UPROPERTY(EditAnywhere)
	class UBlackboardComponent* BlackBoardComp;


public:
	AAIC_BossBase();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};

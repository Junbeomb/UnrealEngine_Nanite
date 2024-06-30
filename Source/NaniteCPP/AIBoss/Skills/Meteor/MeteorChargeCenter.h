// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorChargeCenter.generated.h"

DECLARE_MULTICAST_DELEGATE(F_MCenter);

UCLASS()
class NANITECPP_API AMeteorChargeCenter : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;
	
public:	
	// Sets default values for this actor's properties
	AMeteorChargeCenter();

	F_MCenter D_AbsortStart;
	F_MCenter D_Bomb;

	UFUNCTION()
	void CallDAbsortStart();

	UFUNCTION()
	void CallDBombStart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorRock.generated.h"

UCLASS()
class NANITECPP_API AMeteorRock : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeteorMesh;

	UPROPERTY(EditDefaultsOnly)
	class AMeteorChargeCenter* Center;

	UFUNCTION()
	void AbsorbedByCenter();
	
public:	
	AMeteorRock();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHomingBallBase> SoulBallChoice;

	UFUNCTION()
	void MeteorCenterBind();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

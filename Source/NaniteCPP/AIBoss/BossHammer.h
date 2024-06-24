// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossHammer.generated.h"

UCLASS()
class NANITECPP_API ABossHammer : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HammerStatic;
	
public:	
	// Sets default values for this actor's properties
	ABossHammer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

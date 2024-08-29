// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../BaseProjectile.h"

#include "BlackholeProjectile.generated.h"

UCLASS()
class NANITECPP_API ABlackholeProjectile : public ABaseProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Blackhole")
	TSubclassOf<AActor> BPBlackhole;

protected:
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	void BeginPlay() override;

public:	
	ABlackholeProjectile();

};

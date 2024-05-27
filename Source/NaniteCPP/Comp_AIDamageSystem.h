// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Comp_AIDamageSystem.generated.h"

DECLARE_DELEGATE(FD_Damage);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NANITECPP_API UComp_AIDamageSystem : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void TakeDamage(AActor* DamageCauser, float amount);

	UPROPERTY()
	float health;
	UPROPERTY()
	bool isDead;




public:	
	// Sets default values for this component's properties
	UComp_AIDamageSystem();

	FD_Damage D_OnDeath;
	FD_Damage D_OnDamageResponse;
		
};

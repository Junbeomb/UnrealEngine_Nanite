// Fill out your copyright notice in the Description page of Project Settings.


#include "Comp_AIDamageSystem.h"



// Sets default values for this component's properties
UComp_AIDamageSystem::UComp_AIDamageSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	isDead = false;
	// ...
}

void UComp_AIDamageSystem::TakeDamage(AActor* DamageCauser, float amount)
{
	health -= amount;

	if (health <= 0) {
		isDead = true;
		D_OnDeath.Execute();
	}
	else {
		D_OnDamageResponse.Execute();
	}

}

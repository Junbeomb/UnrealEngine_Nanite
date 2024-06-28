// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHomingBall.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"

void ABossHomingBall::BeginPlay()
{
	Super::BeginPlay();
	if(!TargetActor)
		TargetActor = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if(TargetActor)
		ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	//D_Hit.Broadcast();
}


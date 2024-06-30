// Fill out your copyright notice in the Description page of Project Settings.


#include "MeteorSoulBall.h"
#include "MeteorChargeCenter.h"



void AMeteorSoulBall::BeginPlay()
{
	Super::BeginPlay();

	if (!TargetActor)
		TargetActor = UGameplayStatics::GetActorOfClass(GetWorld(),AMeteorChargeCenter::StaticClass());
	if (TargetActor)
		ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	
	MCenter = Cast<AMeteorChargeCenter>(TargetActor);
	MCenter->D_Bomb.AddUObject(this, &AMeteorSoulBall::Dissapear);
}

void AMeteorSoulBall::Dissapear()
{
	Destroy();
}
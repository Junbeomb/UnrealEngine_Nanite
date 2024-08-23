// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeLightBase.h"
#include "BlackholeCompBase.h"

// Sets default values
ABlackholeLightBase::ABlackholeLightBase()
{
	RotationStrength = 20.f;
    BlackholeComp->PullStrength = 120.f;
    BlackholeComp->LinearDampingAmount = 1.2f;
    BaseStaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    BaseStaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
    BaseStaticMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
}


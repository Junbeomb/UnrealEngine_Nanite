// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageRockBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Blending/Comp_BlendMesh.h"
#include "InstancedFoliageActor.h"
#include "FoliageInfluencer.h"

AFoliageRockBase::AFoliageRockBase()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}


void AFoliageRockBase::GetMeshName(FString& LeftS, FString& RightS)
{
	MeshComponent->GetStaticMesh()->GetName().Split(TEXT("SM_"), &LeftS, &RightS);
}

void AFoliageRockBase::SetCollisionSimulate()
{
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);
}

void AFoliageRockBase::AddFoliageInstance(UActorComponent* AC)
{
	Cast<UInstancedStaticMeshComponent>(AC)->AddInstance(MeshComponent->GetComponentTransform(), true);
}

void AFoliageRockBase::GoCustomFunc()
{
	NoInfluencersInRangeFunc();
}

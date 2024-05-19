// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleSwitchBox.h"
#include "../../Blending/Comp_BlendMesh.h"



// Sets default values
APuzzleSwitchBox::APuzzleSwitchBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box_Mesh"));
	RootComponent = BoxMesh;

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

}

// Called when the game starts or when spawned
void APuzzleSwitchBox::BeginPlay()
{
	Super::BeginPlay();

	Comp_Blend->D_FinishBlending.BindUObject(this, &APuzzleSwitchBox::OnFinishBlending);
}

void APuzzleSwitchBox::OnFinishBlending()
{
	SwitchBoxOn.Broadcast();
}


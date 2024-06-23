// Fill out your copyright notice in the Description page of Project Settings.


#include "BOssTest.h"

// Sets default values
ABOssTest::ABOssTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABOssTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABOssTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABOssTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


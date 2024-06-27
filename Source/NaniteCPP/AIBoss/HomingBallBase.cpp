// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingBallBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "BOssTest.h"

// Sets default values
AHomingBallBase::AHomingBallBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = BallMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

// Called when the game starts or when spawned
void AHomingBallBase::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement->HomingTargetComponent = UGameplayStatics::GetActorOfClass(GetWorld(), ABOssTest::StaticClass())->GetRootComponent();
}

// Called every frame
void AHomingBallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


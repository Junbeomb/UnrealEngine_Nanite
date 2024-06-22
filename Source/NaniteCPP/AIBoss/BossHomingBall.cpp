// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHomingBall.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"
// Sets default values
ABossHomingBall::ABossHomingBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(SphereComp);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(SphereComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 0.1f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	//SphereComp ¶û ºÎµúÈ÷¸é
	SphereComp->OnComponentHit.AddDynamic(this, &ABossHomingBall::OnComponentHit);

}

void ABossHomingBall::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnComponentHit!!"));
	ANaniteCPPCharacter* tempCharac = Cast<ANaniteCPPCharacter>(OtherActor);
	tempCharac->DecreaseHP();
	Destroy();
	//D_Hit.Broadcast();
}



// Called when the game starts or when spawned
void ABossHomingBall::BeginPlay()
{
	Super::BeginPlay();
	Target = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if(Target)
		ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
	
	//D_Hit.Broadcast();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBossBase.h"
#include "Comp_AIDamageSystem.h"
#include "Comp_AIBossAttackSystem.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../Blending/Comp_BlendMesh.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIC_BossBase.h"
#include "BossAttackStructData.h"

// Sets default values
AAIBossBase::AAIBossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = ACharacter::GetCapsuleComponent();
	RootComponent = CapsuleComp;

	SkeletalMesh = ACharacter::GetMesh();
	SkeletalMesh->SetupAttachment(RootComponent);

	Comp_Attack = CreateDefaultSubobject<UComp_AIBossAttackSystem>(TEXT("Comp_Attack"));
	Comp_Damage = CreateDefaultSubobject<UComp_AIDamageSystem>(TEXT("Comp_Damage"));
	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

	//Dissolve Timeline
	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimeline"));
	//callback함수bind
	floatTimelineCallback.BindUFunction(this, FName("DissolveTimelineUpdate"));
	floatTimelineFinishedCallback.BindUFunction(this, FName("DissolveTimelineFinish"));

	//aicontroller 설정
	AIControllerClass = AAIC_BossBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}



// Called when the game starts or when spawned
void AAIBossBase::BeginPlay()
{
	Super::BeginPlay();

	//AttackCombo1(nullptr);
	ThrowBall(nullptr);

	if (!BehaviorTree) {
		UE_LOG(LogTemp, Warning, TEXT("BT Empty"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s"), GetController()->GetName());

	for (int i = 0; i < SkeletalMesh->GetNumMaterials(); i++) {
		 UMaterialInstanceDynamic* TempDMI = SkeletalMesh->CreateDynamicMaterialInstance(i, SkeletalMesh->GetMaterial(i));
		 DMIList.Add(TempDMI);
	}

	Comp_Damage->D_OnDeath.BindUObject(this, &AAIBossBase::Die);
	Comp_Damage->D_OnDamageResponse.BindUObject(this, &AAIBossBase::HitResponse);

}

void AAIBossBase::Die()
{
	Comp_Blend->StartBlend();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//AIC_BossBase->StopMovement();
	GetController()->StopMovement();

	SkeletalMesh->SetSimulatePhysics(true);
	
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			DissolveTimeline->AddInterpFloat(DissolveCurve, floatTimelineCallback, FName("DissolveAmountTL"));
			DissolveTimeline->SetTimelineFinishedFunc(floatTimelineFinishedCallback);
			DissolveTimeline->PlayFromStart();
		});

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, 3, false);
}

void AAIBossBase::DissolveTimelineFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("AIBossBase Dissolve Timeline Finish"));
}

void AAIBossBase::DissolveTimelineUpdate(float Value)
{
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetScalarParameterValue(FName("DissolveAmount"), Value);
	}
}

void AAIBossBase::HitResponse()
{
	GetCharacterMovement()->StopMovementImmediately();
}

// Called to bind functionality to input
void AAIBossBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AAIBossBase::AttackCombo1(AActor* ATarget)
{
	FBOSSATTACKDATA TempAData;
	TempAData.AttackTarget = ATarget;
	TempAData.DamageAmount = 11;
	TempAData.radius = 30.f;
	TempAData.length = 50.f;
	TempAData.Montage = Combo1Montage;
	UE_LOG(LogTemp, Warning, TEXT("AttackCombo1() in AIBossBase.cpp"));
	Comp_Attack->BossPrimaryAttack(TempAData);
}

void AAIBossBase::ThrowBall(AActor* ATarget)
{
	FBOSSATTACKDATA TempAData;
	TempAData.AttackTarget = ATarget;
	TempAData.DamageAmount = 20;
	TempAData.radius = 30.f;
	TempAData.length = 50.f;
	TempAData.Montage = ThrowBallMontage;
	UE_LOG(LogTemp, Warning, TEXT("ThrowBall() in AIBossBase.cpp"));
	Comp_Attack->BossThrowBall(TempAData);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBossBase.h"
#include "Comp_AIDamageSystem.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blending/Comp_BlendMesh.h"

// Sets default values
AAIBossBase::AAIBossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = ACharacter::GetCapsuleComponent();
	RootComponent = CapsuleComp;

	SkeletalMesh = ACharacter::GetMesh();
	SkeletalMesh->SetupAttachment(RootComponent);

	Comp_Damage = CreateDefaultSubobject<UComp_AIDamageSystem>(TEXT("Comp_Damage"));
	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

	//Dissolve Timeline
	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimeline"));
	//callbackÇÔ¼öbind
	floatTimelineCallback.BindUFunction(this, FName("DissolveTimelineUpdate"));
	floatTimelineFinishedCallback.BindUFunction(this, FName("DissolveTimelineFinish"));


}

// Called when the game starts or when spawned
void AAIBossBase::BeginPlay()
{
	Super::BeginPlay();

	if (!AIC_BossBase || !BehaviorTree) {
		UE_LOG(LogTemp, Warning, TEXT("AIC_BossBase or BT Empty"));
		return;
	}

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
	AIC_BossBase->StopMovement();

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
}

// Called to bind functionality to input
void AAIBossBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


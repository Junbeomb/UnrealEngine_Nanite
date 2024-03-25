// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeCompBase.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UBlackholeCompBase::UBlackholeCompBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	PullStrength = 150.f;
	LinearDampingAmount = 0.8f;
	SmallScale = 0.1f;
	// ...
}


// Called when the game starts
void UBlackholeCompBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBlackholeCompBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetDeltaTime(DeltaTime);

	if (GetIsPull()) {
		PullDirection = DirectBH(); //액터와 블랙홀 간의 방향 최신화

		if (GetIsShrink()) {
			GetOwner()->SetActorLocation(PullTargetLocation);
		}
		else {
			//블랙홀 과의 거리 0~1 로 만든 값 저장
			DistanceNormalized = UKismetMathLibrary::MapRangeClamped(PullDirection.Length(), 0, PullStartDistanceToBlackhole, 0, 1);

			//주인 액터의 staticmeshcomponent 얻어오기
			UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			//빨아들이기
			FVector ForceValue = PullDirection * (DistanceNormalized / 5 + (1 - DistanceNormalized)) * PullStrength * GetDeltaTime();
			SMC->AddForce(ForceValue, "None", true);
			//damping 주기
			SMC->SetLinearDamping(DistanceNormalized * LinearDampingAmount);
			//점점 작아지기
			float TempScale = UKismetMathLibrary::MapRangeClamped(DistanceNormalized, 0, 1, SmallScale, InitialMaxScale.X);
			SMC->SetWorldScale3D({ TempScale,TempScale,TempScale });
		}

	}
}

void UBlackholeCompBase::SetPullOn(ABlackhole* BH, FVector BHLocation)
{

	//UE_LOG(LogTemp, Warning, TEXT("%f"), BHLocation.X);


	SetPullStartDistance();
	SetInitialMaxScale();

	SetIsPull(true);
	SetIsShrink(false);
}



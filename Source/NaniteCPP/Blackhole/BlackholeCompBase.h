// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blackhole.h"
#include "BlackholeCompBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NANITECPP_API UBlackholeCompBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlackholeCompBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetDeltaTime(float a) { TickDeltaTime = a; };
	const float GetDeltaTime() { return TickDeltaTime; };

	void SetIsPull(bool a) { IsPull = a; };
	const bool GetIsPull() { return IsPull; };

	void SetIsShrink(bool a) { IsShrink = a; };
	const bool GetIsShrink() { return IsShrink; };

	void SetBlackhole(ABlackhole* a) { Blackhole = a; };
	void SetPullTargetLocation(FVector a) { PullTargetLocation = a; };

	FVector DirectBH() { 
		//블랙홀과의 거리
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		return PullTargetLocation - (SMC->GetComponentLocation());
	};

	void SetPullStartDistance() { PullStartDistanceToBlackhole = DirectBH().Length(); };

	void SetInitialMaxScale() { 
		UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())); 
		InitialMaxScale = Temp->GetRelativeScale3D();
	}

	//처음 빨려들어갈시 변수들 초기화하는 함수
	void SetPullOn(ABlackhole* BH, FVector BHLocation);

private:
	UPROPERTY()
	float TickDeltaTime;
	ABlackhole* Blackhole;

	//빨려들어가는지
	UPROPERTY()
	bool IsPull;
	//블랙홀의 위치
	FVector PullTargetLocation;
	//현재 액터와 블랙홀의 방향(회전하는 방향 결정)
	UPROPERTY(VisibleAnywhere)
	FVector PullDirection;
	//현재 엑터와 블랙홀의 처음 시작 거리
	float PullStartDistanceToBlackhole;
	//액터와 블랙홀간의 거리(0~1)
	float DistanceNormalized;

	//생성자에서 값 직접 생성
	UPROPERTY(EditAnywhere)
	float PullStrength;
	UPROPERTY(EditAnywhere)
	float LinearDampingAmount;
	UPROPERTY(EditAnywhere)
	float SmallScale;

	FVector InitialMaxScale;
	FRotator InitialRotationDegree;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DMIList;


	//줄어들고 있는지
	UPROPERTY()
	bool IsShrink;

		
};

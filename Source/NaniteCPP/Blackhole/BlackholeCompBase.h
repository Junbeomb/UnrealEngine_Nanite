// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blackhole.h"
#include "NiagaraComponent.h"
#include "BlackholeCompBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NANITECPP_API UBlackholeCompBase : public UActorComponent
{
	GENERATED_BODY()

	void SetPullTargetLocation(FVector a) { PullTargetLocation = a; };

	FVector DirectBH();

	//해당 거리 안에 들어오는지
	bool IsDistanceToBH(float CriteriaDistance) {
		return DirectBH().Length() <= CriteriaDistance;
	}

	void SetPullStartDistance() { PullStartDistanceToBlackhole = DirectBH().Length(); };

	void SetInitialMaxScale() {
		UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		InitialMaxScale = Temp->GetRelativeScale3D();
	}

	void SetInitialNSSpawnRate();

	UPROPERTY()
	UStaticMeshComponent* SMC;

	UPROPERTY()
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


	//곧 Destroy됨
	UPROPERTY()
	bool IsWillDie;
	//줄어들고 있는지
	UPROPERTY()
	bool IsShrink;

	//나이아가라
	UNiagaraComponent* NiagaraComp;
	UPROPERTY(EditAnywhere, Category = "Niagara")
	float InitialNSVortexForceAmount;
	UPROPERTY(EditAnywhere, Category = "Niagara")
	float InitialNSSpawnRate;

	float DecreaseTime;
	float DecreaseTimeTotal;
	float DecreaseSpawnRateTime;
	float DecreaseSpawnRateTimeTotal;

	//나이아가라 늦게 시작
	UPROPERTY()
	bool IsLateNiagaraSpawnToggle;
	UPROPERTY()
	bool IsVisibleNiagara;
	//DFStartRadius
	UPROPERTY()
	float DFStartDistance;

public:	
	// Sets default values for this component's properties
	UBlackholeCompBase();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//처음 빨려들어갈시 변수들 초기화하는 함수
	void SetPullOn(ABlackhole* BH, FVector BHLocation);
	const bool GetIsPull() { return IsPull; };
	
	const FRotator GetInitialRotationDegree() { return InitialRotationDegree; };

	void SetDFOff(bool IsOn);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};

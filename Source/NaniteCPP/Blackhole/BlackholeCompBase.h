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

	//�ش� �Ÿ� �ȿ� ��������
	bool IsDistanceToBH(float CriteriaDistance) {
		return DirectBH().Length() <= CriteriaDistance;
	}

	void SetPullStartDistance() { PullStartDistanceToBlackhole = DirectBH().Length(); };

	void SetInitialNSSpawnRate();

	UPROPERTY()
	UStaticMeshComponent* SMC;

	UPROPERTY()
	ABlackhole* Blackhole;

	//����������
	UPROPERTY()
	bool IsPull;
	//��Ȧ�� ��ġ
	FVector PullTargetLocation;
	//���� ���Ϳ� ��Ȧ�� ����(ȸ���ϴ� ���� ����)
	UPROPERTY(VisibleAnywhere)
	FVector PullDirection;
	//���� ���Ϳ� ��Ȧ�� ó�� ���� �Ÿ�
	float PullStartDistanceToBlackhole;
	//���Ϳ� ��Ȧ���� �Ÿ�(0~1)
	float DistanceNormalized;

	//�����ڿ��� �� ���� ����

	UPROPERTY()
	float SmallScale;

	FVector InitialMaxScale;
	FRotator InitialRotationDegree;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DMIList;

	//�پ��� �ִ���
	UPROPERTY()
	bool IsShrink;

	//���̾ư���
	UNiagaraComponent* NiagaraComp;
	UPROPERTY(EditAnywhere, Category = "Niagara")
	float InitialNSVortexForceAmount;
	UPROPERTY(EditAnywhere, Category = "Niagara")
	float InitialNSSpawnRate;

	float DecreaseTime;
	float DecreaseTimeTotal;
	float DecreaseSpawnRateTime;
	float DecreaseSpawnRateTimeTotal;

	//���̾ư��� �ʰ� ����
	UPROPERTY()
	bool IsLateNiagaraSpawnToggle;
	UPROPERTY()
	bool IsVisibleNiagara;
	//DFStartRadius
	UPROPERTY()
	float DFStartDistance;

public:	
	UBlackholeCompBase();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//ó�� �������� ������ �ʱ�ȭ�ϴ� �Լ�
	void SetPullOn(ABlackhole* BH, FVector BHLocation);
	const bool GetIsPull() { return IsPull; };
	
	const FRotator GetInitialRotationDegree() { return InitialRotationDegree; };

	void SetDFOff(bool IsOn);

	UPROPERTY()
	float PullStrength;
	UPROPERTY()
	float LinearDampingAmount;

protected:
	virtual void BeginPlay() override;



		
};

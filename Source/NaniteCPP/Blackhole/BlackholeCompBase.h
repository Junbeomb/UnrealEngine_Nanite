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

public:	
	// Sets default values for this component's properties
	UBlackholeCompBase();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//ó�� �������� ������ �ʱ�ȭ�ϴ� �Լ�
	void SetPullOn(ABlackhole* BH, FVector BHLocation);
	const bool GetIsPull() { return IsPull; };
	
	const FRotator GetInitialRotationDegree() { return InitialRotationDegree; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void SetPullTargetLocation(FVector a) { PullTargetLocation = a; };

	FVector DirectBH() { 
		//��Ȧ���� �Ÿ�,����
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		return PullTargetLocation - (SMC->GetComponentLocation());
	};

	//�ش� �Ÿ� �ȿ� ��������
	bool IsDistanceToBH(float CriteriaDistance) {
		return DirectBH().Length() <= CriteriaDistance;
	}

	void SetPullStartDistance() { PullStartDistanceToBlackhole = DirectBH().Length(); };

	void SetInitialMaxScale() { 
		UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())); 
		InitialMaxScale = Temp->GetRelativeScale3D();
	}

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


	//�� Destroy��
	UPROPERTY()
	bool IsWillDie;
	//�پ��� �ִ���
	UPROPERTY()
	bool IsShrink;

	//���̾ư��� �ʰ� ����
	UNiagaraComponent* NiagaraComp;
	float InitialNSVortexForceAmount;
	float InitialNSSpawnRate;
	UPROPERTY()
	bool IsLateNiagaraSpawnToggle;
	UPROPERTY()
	bool IsVisibleNiagara;
	//DFStartRadius
	UPROPERTY()
	float DFStartDistance;

		
};

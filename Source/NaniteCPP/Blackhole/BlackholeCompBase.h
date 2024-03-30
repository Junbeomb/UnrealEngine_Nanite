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

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//ó�� �������� ������ �ʱ�ȭ�ϴ� �Լ�
	void SetPullOn(ABlackhole* BH, FVector BHLocation);
	const bool GetIsPull() { return IsPull; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void SetDeltaTime(float a) { TickDeltaTime = a; };
	const float GetDeltaTime() { return TickDeltaTime; };

	void SetIsPull(bool a) { IsPull = a; };

	void SetIsShrink(bool a) { IsShrink = a; };
	const bool GetIsShrink() { return IsShrink; };

	void SetBlackhole(ABlackhole* a) { Blackhole = a; };
	void SetPullTargetLocation(FVector a) { PullTargetLocation = a; };

	FVector DirectBH() { 
		//��Ȧ���� �Ÿ�
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		return PullTargetLocation - (SMC->GetComponentLocation());
	};

	void SetPullStartDistance() { PullStartDistanceToBlackhole = DirectBH().Length(); };

	void SetInitialMaxScale() { 
		UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())); 
		InitialMaxScale = Temp->GetRelativeScale3D();
	}

	UPROPERTY()
	float TickDeltaTime;
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


	//�پ��� �ִ���
	UPROPERTY()
	bool IsShrink;

		
};

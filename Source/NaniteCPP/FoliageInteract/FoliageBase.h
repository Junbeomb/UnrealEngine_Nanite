// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoliageBase.generated.h"

UCLASS()
class NANITECPP_API AFoliageBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoliageBase();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

private:
	//���忡 �ִ� FoliageInfluencers ������
	UPROPERTY(EditAnywhere, category = "Physics")
	TArray<AActor*> AllFoliageInfluencers;
	UPROPERTY(EditAnywhere, category = "Physics")
	int FoliageInfluencersLen;
	UPROPERTY(EditAnywhere, category = "Physics")
	bool IsInfluencersInRange;
	UFUNCTION()
	void NoInfluencersInRangeFunc();

	//���忡�ִ� Foliage ��������
	class AInstancedFoliageActor* WorldFoliage;

	class UComp_BlendMesh* Comp_Blend;

	void ReturnToFoliage();

	//FoliageInfluencer�� ������ �ִ��� Ȯ��
	UFUNCTION()
	void checkToFoliageInfluencer();

	UPROPERTY()
	bool DoOnce;

	UPROPERTY(EditAnywhere)
	bool FinishDoOnce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

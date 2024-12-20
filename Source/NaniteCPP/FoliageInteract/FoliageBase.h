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
	UStaticMeshComponent* StMeshComponent;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkMeshComponent;

protected:
	void BeginPlay() override;

	virtual void SetCollisionSimulate() {};
	virtual void GetMeshName(FString& LeftS, FString& RightS) {};
	virtual void AddFoliageInstance(UActorComponent*) {};
	virtual void GoCustomFunc() {};
	virtual void TurnOffAnim() {};
	virtual void DestroyCompFunc() {};

	//월드에 있는 FoliageInfluencers 정보들
	UPROPERTY(EditAnywhere, category = "Physics")
	TArray<AActor*> AllFoliageInfluencers;
	UPROPERTY(EditAnywhere, category = "Physics")
	int FoliageInfluencersLen;
	UPROPERTY(EditAnywhere, category = "Physics")
	bool IsInfluencersInRange;
	UFUNCTION()
	virtual void NoInfluencersInRangeFunc();

	//월드에있는 Foliage 가져오기
	class AInstancedFoliageActor* WorldFoliage;

	class UComp_BlendMesh* Comp_Blend;


	UFUNCTION()
	virtual void ReturnToFoliage();

	//FoliageInfluencer가 주위에 있는지 확인
	UFUNCTION()
	virtual void checkToFoliageInfluencer();

	UPROPERTY()
	bool DoOnce;

	UPROPERTY(EditAnywhere)
	bool FinishDoOnce;



};

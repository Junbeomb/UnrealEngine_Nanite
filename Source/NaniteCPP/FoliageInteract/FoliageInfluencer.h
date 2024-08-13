// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoliageInfluencer.generated.h"

UENUM(BlueprintType)
enum class EFoliageType : uint8
{
	JustGo UMETA(DisplayName = "JustGo"),
	Blackhole UMETA(DisplayName = "Blackhole"),
	ChangeBlend UMETA(DisplayName = "ChangeBlend")
};


UCLASS()
class NANITECPP_API AFoliageInfluencer : public AActor
{
	GENERATED_BODY()

	AFoliageInfluencer();

	UPROPERTY(EditAnywhere, Category="Foliage")
	TArray<TSubclassOf<class AFoliageBase>> FoliageBlueprints; //TSubclassOf : 월드내에 배치되어있는 actor 이외에도 다 가져올 수 있음.
	UPROPERTY(EditAnywhere, Category = "Foliage")
	TArray<TSubclassOf<class ABlackholeActorBase>> BlackholeFoliageBlueprints; //TSubclassOf : 월드내에 배치되어있는 actor 이외에도 다 가져올 수 있음.

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* FoliageTransMesh;

	UPROPERTY(EditAnywhere, Category="Blending")
	TSubclassOf<class AMaterialChangeBall> MaterialChangeBall;


	//부딪힌component, actor들을 찾는 함수
	bool FindTrace();
	TArray<FHitResult> OutResults;

	UFUNCTION()
	void CheckBlend(AActor* CheckActor, FVector ImpactPoint);
	UPROPERTY(EditAnywhere,Category="Foliage")
	EFoliageType FoliageType;

	FActorSpawnParameters ActorSpawnParams;

	UFUNCTION()
	void SpawnAndConvert(UMeshComponent* TempMesh);
	FString value;
	FTransform InstanceTransform;
	UInstancedStaticMeshComponent* InstancedMeshComp;

public:	
	virtual void Tick(float DeltaTime) override;

	//LineTrace Radius;
	UPROPERTY(EditAnywhere, Category = "Foliage")
	float PhysicsRadius;
	UPROPERTY(EditAnywhere,Category="Foliage")
	bool IsBlackholeInfluencer;
};

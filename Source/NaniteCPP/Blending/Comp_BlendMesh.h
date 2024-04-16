// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Comp_BlendMesh.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FD_Delegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NANITECPP_API UComp_BlendMesh : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComp_BlendMesh();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartBlend();
	UFUNCTION()
	void JustGo();

	UFUNCTION()
	UMeshComponent* StaticOrSkeletal();
	UFUNCTION()
	bool IsLow();

	UFUNCTION()
	void CreateDMIAndDFOff(UPrimitiveComponent* UComp,int NumMaterial);

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FD_Delegate D_FinishBlending;


private:

	UStaticMeshComponent* SMC;
	USkeletalMeshComponent* SKC;

	TArray<UMaterialInstanceDynamic*> DMIList;

	bool IsTickStart;
	bool IsBlendStart;

	bool IsHighQuality;

	bool OwnerIsStatic;

	float SumSeconds;

	float WhichOneIsLongestXYZ;

	float ExtentSubtractAmountOneSecond;

		
};

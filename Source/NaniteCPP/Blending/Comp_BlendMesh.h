// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaniteCPP/BeginnerCharacter/NaniteCPPCharacter.h"
#include "../InteractionSystem/InteractStatue.h"
#include "Comp_BlendMesh.generated.h"

DECLARE_DELEGATE(FD_Blend);
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
	void StartBlendMass(FVector CenterLocation,AInteractStatue* InteractStatue );
	UFUNCTION()
	void StartBlendMassTickStartFunc();
	UFUNCTION()
	void StartBlendMassTickFinishFunc();
	UFUNCTION()
	void JustGo();

	UFUNCTION()
	UMeshComponent* StaticOrSkeletal();
	UFUNCTION()
	bool IsLow();

	UFUNCTION()
	void CreateDMIAndDFOff(UPrimitiveComponent* UComp,int NumMaterial);


	FD_Blend D_FinishBlending;
	FD_Blend D_JustGo;

	bool IsBlendStart;

	float ExtentSubtractAmountOneSecond;

private:

	void FinishBlendSetVariable();

	ANaniteCPPCharacter* Player;

	UStaticMeshComponent* SMC;
	USkeletalMeshComponent* SKC;

	TArray<UMaterialInstanceDynamic*> DMIList;

	AInteractStatue* BlendMassStatue;

	bool IsTickStart;
	bool IsMassTickStart;


	bool IsHighQuality;

	bool OwnerIsStatic;

	float SumSeconds;

	float WhichOneIsLongestXYZ;


		
};

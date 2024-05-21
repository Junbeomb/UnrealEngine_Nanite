// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "Interface_Interact.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#include "Components/TimelineComponent.h"

#include "InteractStatueBig.generated.h"

UCLASS()
class NANITECPP_API AInteractStatueBig : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WindNS;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* Bomb;

	UPROPERTY(EditAnywhere)
	float BombDistance;
	float BlendRadius;


	FLinearColor InitialEmissiveColor;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* ParamCollection;
	UMaterialParameterCollectionInstance* PCI;

	UPROPERTY(EditAnywhere)
	class APostProcessVolume* PPV;
	float PPVIntensity;
	float PPVCompensation;


	TArray<UInstancedStaticMeshComponent*> InstancedMesh;

	TArray<UMaterialInstanceDynamic*> DMIList;

	TArray<UMaterialInstanceDynamic*> ShakeSMDMIList;

	void StartMassBlend();

	//MassBlendTimeline
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* MassBlendTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* MassBlendCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* NormalAmplifyCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* EmissiveCurve;
	//종료시
	FOnTimelineEvent MassBlendTimelineFinishedCallback;
	UFUNCTION()
		void SetMassBlendTimelineFinish();
	//callback함수
	FOnTimelineFloat MassBlendTimelineUpdateCallback;
	UFUNCTION() //UFUNCTION()이 있어야 동작한다.!!!!!!
		void SetMassBlendTimelineUpdate(float Value);
	FOnTimelineFloat NormalAmplifyTimelineUpdateCallback;
	UFUNCTION()
		void SetNormalAmplifyTimelineUpdate(float Value);
	FOnTimelineFloat EmissiveTimelineUpdateCallback;
	UFUNCTION()
		void SetEmissiveTimelineUpdate(float Value);

	//ShakeStaticMeshTimeline
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* ShakeSMTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* ShakeSMCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* SizeSMCurve;
	//종료시
	FOnTimelineEvent ShakeSMTimelineFinishedCallback;
	UFUNCTION()
		void SetShakeSMTimelineFinish();
	//callback함수
	FOnTimelineFloat ShakeSMTimelineUpdateCallback;
	UFUNCTION() //UFUNCTION()이 있어야 동작한다.!!!!!!
		void SetShakeSMTimelineUpdate(float Value);
	FOnTimelineFloat SizeSMTimelineUpdateCallback;
	UFUNCTION() //UFUNCTION()이 있어야 동작한다.!!!!!!
		void SetSizeSMTimelineUpdate(float Value);

	//PostProcessTimeline
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* PPVTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* PPVCurve;
	//종료시
	FOnTimelineEvent PPVTimelineFinishedCallback;
	UFUNCTION()
		void SetPPVTimelineFinish();
	//callback함수
	FOnTimelineFloat PPVTimelineUpdateCallback;
	UFUNCTION() //UFUNCTION()이 있어야 동작한다.!!!!!!
		void SetPPVTimelineUpdate(float Value);
	
public:	
	// Sets default values for this actor's properties
	AInteractStatueBig();

	UPROPERTY(EditAnywhere)
	class UComp_InteractBase* CompBase;

	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

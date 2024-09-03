#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Comp_InteractBase.h"
#include "Interface_Interact.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#include "Components/TimelineComponent.h"

#include "InteractStatue.generated.h"

DECLARE_MULTICAST_DELEGATE(FD_BPDelegate);
UCLASS()
class NANITECPP_API AInteractStatue : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WindNS;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* Bomb;

	float BombDistance;
	float BlendRadius;



	FLinearColor InitialEmissiveColor;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* ParamCollection;
	UMaterialParameterCollectionInstance* PCI;


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

		//�����
		FOnTimelineEvent MassBlendTimelineFinishedCallback;
		UFUNCTION()
		void SetMassBlendTimelineFinish();

		//callback�Լ�
		FOnTimelineFloat MassBlendTimelineUpdateCallback;
		UFUNCTION() //UFUNCTION()�� �־�� �����Ѵ�.!!!!!!
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
		//�����
		FOnTimelineEvent ShakeSMTimelineFinishedCallback;
		UFUNCTION()
		void SetShakeSMTimelineFinish();
		//callback�Լ�
		FOnTimelineFloat ShakeSMTimelineUpdateCallback;
		UFUNCTION() //UFUNCTION()�� �־�� �����Ѵ�.!!!!!!
		void SetShakeSMTimelineUpdate(float Value);
		FOnTimelineFloat SizeSMTimelineUpdateCallback;
		UFUNCTION() //UFUNCTION()�� �־�� �����Ѵ�.!!!!!!
		void SetSizeSMTimelineUpdate(float Value);


protected:
	virtual void BeginPlay() override;

public:	
	AInteractStatue();

	FD_BPDelegate D_Die;

	UComp_InteractBase* CompBase;

	virtual void PressEStart() override;



};

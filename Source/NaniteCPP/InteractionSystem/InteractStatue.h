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

DECLARE_DELEGATE(FD_BPDelegate);
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

	UPROPERTY(EditAnywhere)
	class USphereComponent* CheckInstanceSMC;

	UPROPERTY(EditAnywhere)
	float BombDistance;
	float BlendRadius;



	FLinearColor InitialEmissiveColor;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* ParamCollection;
	UMaterialParameterCollectionInstance* PCI;


	TArray<UInstancedStaticMeshComponent*> InstancedMesh;

	TArray<UMaterialInstanceDynamic*> DMIList;

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

	//SMCSPhereTimeline
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* SMCSphereTimeline;
		UPROPERTY(EditAnywhere,Category="Timeline")
		UCurveFloat* SMCSphereCurve;
		//�����
		FOnTimelineEvent SMCSphereTimelineFinishedCallback;
		UFUNCTION()
		void SetSMCSphereTimelineFinish();
		FOnTimelineFloat SMCSphereTimelineUpdateCallback;
		UFUNCTION()
		void SetSMCSphereTimelineUpdate(float Value);

		//callback�Լ�
	
public:	
	// Sets default values for this actor's properties
	AInteractStatue();

	UPROPERTY(EditAnywhere)
	UComp_InteractBase* CompBase;

	virtual void PressEStart() override;

	FD_BPDelegate D_BPStartBlend;
	FD_BPDelegate D_BPFinishBlend;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

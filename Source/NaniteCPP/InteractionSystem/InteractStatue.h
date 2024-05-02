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

	UFUNCTION()
	void OverlapSMCRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere)
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

	//SMCSPhereTimeline
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* SMCSphereTimeline;
		UPROPERTY(EditAnywhere,Category="Timeline")
		UCurveFloat* SMCSphereCurve;
		//종료시
		FOnTimelineEvent SMCSphereTimelineFinishedCallback;
		UFUNCTION()
		void SetSMCSphereTimelineFinish();
		//callback함수
		FOnTimelineFloat SMCSphereTimelineUpdateCallback;
		UFUNCTION()
		void SetSMCSphereTimelineUpdate(float Value);


	
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

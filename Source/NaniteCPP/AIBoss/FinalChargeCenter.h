
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "FinalChargeCenter.generated.h"

UCLASS()
class NANITECPP_API AFinalChargeCenter : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CenterMesh;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;

	UPROPERTY()
	UMaterialInstanceDynamic* DMI;


private://Blend
	UPROPERTY()
	class UComp_BlendMesh* Comp_Blend;

	UFUNCTION()
	void BlendStart();
	UFUNCTION()
	void BlendFinish();

private://Timeline

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* TL_Dissapear;

	//TL_Dissapear
		//TL_Dissapear 종료시
		FOnTimelineEvent TLDissapearFinishCallback;
		UFUNCTION()
		void TLDissapearFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* EmissiveCurve;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* WPOCurve;
	//callback함수
	FOnTimelineFloat EmissiveCallback;
	//callback함수
	FOnTimelineFloat WPOCallback;
	//Bind함수
	UFUNCTION()
	void EmissiveUpdate(float Value);
	UFUNCTION()
	void WPOUpdate(float Value);

	
public:	
	AFinalChargeCenter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindBomb.generated.h"

class ABlackhole;
class AInteractStatue;
UCLASS()
class NANITECPP_API AWindBomb : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	class AWindManager* windManager;

	void StartWind();

	float Duration;
	float Radius;
	float Strength;

	UPROPERTY(EditAnywhere)
	UCurveFloat* StrengthCurve;


	//Timeline
	UTimelineComponent* WindTimeline;
	FOnTimelineEvent WindTLFinishedCallback;
	UFUNCTION()
	void SetScaleTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* StatueRadiusCurve;
	FOnTimelineFloat RadiusTLCallback;
	UFUNCTION()
	void SetRadiusTLUpdate(float Value);

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* StatueStrengthCurve;
	FOnTimelineFloat StrengthTLCallback;
	UFUNCTION()
	void SetStrengthTLUpdate(float Value);

public:	
	AWindBomb();

	void BlackholeFunc(ABlackhole& bh);
	void StatueFunc(AInteractStatue& is);

};

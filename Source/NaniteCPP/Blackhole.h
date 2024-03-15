
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Blackhole.generated.h"

UCLASS()
class NANITECPP_API ABlackhole : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABlackhole();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BlackholeBaseMesh;

	class UTimelineComponent* MeshTimeline;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	float TimelineLength;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"));
	class UCurveFloat* FloatCurve;

	//callback함수
	FOnTimelineFloat floatTimelineCallback;
	FOnTimelineEvent floatTimelineFinishedCallback;

	//Bind함수
	UFUNCTION()
	void SetScaleTimelineUpdate(float Value);
	UFUNCTION()
	void SetScaleTimelineFinish();

};

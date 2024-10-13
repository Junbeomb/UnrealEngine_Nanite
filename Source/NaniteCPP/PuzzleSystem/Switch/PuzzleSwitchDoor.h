
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PuzzleSwitchDoor.generated.h"

DECLARE_MULTICAST_DELEGATE(FD_SwitchDoor);
UCLASS()
class NANITECPP_API APuzzleSwitchDoor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere)
	TArray<int> doorKeyNumArr;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<int> doorArr;

	UPROPERTY() //GetActorOfClass()로 월드에 있는 액터로 값을 초기화할때는 댕글링 포인터 방지를 위해 UPROPERTY()를 붙여주기.
	class APuzzleSwitchBox* SwitchBox;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DMIList;

	//timeline=====
	class UTimelineComponent* EmissiveTimeline;
	FOnTimelineEvent EmissiveTimelineFinishedCallback;
	UFUNCTION()
	void EmissiveTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* EmissiveCurve;
	//callback함수
	FOnTimelineFloat EmissiveTimelineCallback;
	//Bind함수
	UFUNCTION()
	void EmissiveTimelineUpdate(float Value);
	//timeline=====

	void TurnOn();

protected:
	virtual void BeginPlay() override;

public:	
	APuzzleSwitchDoor();
	void pushElementDoorArr(int num);

	FD_SwitchDoor wrongAnswer;

};

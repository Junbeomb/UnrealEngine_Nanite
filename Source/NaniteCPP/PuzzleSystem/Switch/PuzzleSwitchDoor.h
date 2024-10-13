
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

	UPROPERTY() //GetActorOfClass()�� ���忡 �ִ� ���ͷ� ���� �ʱ�ȭ�Ҷ��� ��۸� ������ ������ ���� UPROPERTY()�� �ٿ��ֱ�.
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
	//callback�Լ�
	FOnTimelineFloat EmissiveTimelineCallback;
	//Bind�Լ�
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

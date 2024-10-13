// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "../../InteractionSystem/Interface_Interact.h"
#include "PuzzleSwitchButton.generated.h"

UCLASS()
class NANITECPP_API APuzzleSwitchButton : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int ButtonNum;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY()//NewObject 로 초기화하므로 가비지 컬렉션 대상.
	class UComp_InteractBase* Comp_Interact;
	
	UPROPERTY() //world에 있는 것을 가리키므로 dangling pointer 방지를 위해 UPROPERTY() 붙여주기
	class APuzzleSwitchDoor* SwitchDoor;

	UPROPERTY()
	class APuzzleSwitchBox* SwitchBox;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DMIList;

	FLinearColor colorDefault = { 0.05f,0.05f,0.5f,1.f };
	FLinearColor colorBright = { 0.05f,0.05f,10.f,1.f };
	FLinearColor colorRed = { 0.5f,0.05f,0.05f,1.f };

	//timeline=====
	class UTimelineComponent* EmissiveTimeline;
	FOnTimelineEvent EmissiveTimelineFinishedCallback;
	UFUNCTION()
	void EmissiveTimelineFinish();

	UPROPERTY(EditAnywhere, Category = "Timeline");
	class UCurveFloat* EmissiveCurve;
	FOnTimelineFloat EmissiveTimelineCallback;
	UFUNCTION()
	void EmissiveTimelineUpdate(float Value);
	//timeline=====

	void ResetButton();
	UFUNCTION()
	void TurnOn();
	UFUNCTION()
	void WrongFunc();

protected:
	virtual void BeginPlay() override;

public:	
	APuzzleSwitchButton();

	virtual void PressEStart() override;

};

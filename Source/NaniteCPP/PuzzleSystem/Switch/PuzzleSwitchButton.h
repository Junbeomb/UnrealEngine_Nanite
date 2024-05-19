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

	UPROPERTY(EditAnywhere)
	class UComp_InteractBase* Comp_Interact;
	
	UPROPERTY(EditAnywhere)
	class APuzzleSwitchDoor* SwitchDoor;

	UPROPERTY(EditAnywhere)
	class APuzzleSwitchBox* SwitchBox;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInstanceDynamic*> DMIList;

	FLinearColor colorDefault = { 0.05f,0.05f,0.5f,1.f };
	FLinearColor colorBright = { 0.05f,0.05f,10.f,1.f };
	FLinearColor colorRed = { 0.5f,0.05f,0.05f,1.f };

	//timeline=====
	UPROPERTY(EditAnywhere, Category = "Timeline")
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

	UFUNCTION()
	void ResetButton();

	UFUNCTION()
	void TurnOn();

	UFUNCTION()
	void WrongFunc();

public:	
	// Sets default values for this actor's properties
	APuzzleSwitchButton();


	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};

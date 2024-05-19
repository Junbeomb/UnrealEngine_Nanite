// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UFUNCTION()
	void ResetButton();

	UFUNCTION()
	void TurnOn();

public:	
	// Sets default values for this actor's properties
	APuzzleSwitchButton();


	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};

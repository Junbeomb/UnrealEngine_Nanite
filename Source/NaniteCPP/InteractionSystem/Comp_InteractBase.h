// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface_Interact.h"
#include "Comp_InteractBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NANITECPP_API UComp_InteractBase : public UActorComponent
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	UMaterialInstance* OverlayMaterialInstance;

	UPROPERTY()
	UMaterialInstanceDynamic* DMIOverlay;

	UPROPERTY()
	UStaticMeshComponent* OwnerStatic;

	bool cantToggle{ false };

protected:
	virtual void BeginPlay() override;

public:	
	UComp_InteractBase();

	void TurnOnHover();
	void TurnOffHover();
	void TurnOnToggleFunction();

	void DestroyThisComponentFunc();



	UPROPERTY(EditAnywhere)
	FLinearColor OverlayColor;

	UFUNCTION()
	void SetCantToggle(bool ct);



		
};

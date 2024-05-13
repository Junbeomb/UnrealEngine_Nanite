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
	UMaterialInstance* OverlayMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstanceDynamic* DMIOverlay;

	UPROPERTY(EditAnywhere)
	FLinearColor OverlayColor;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* OwnerStatic;




public:	

	// Sets default values for this component's properties
	UComp_InteractBase();

	UFUNCTION(Category = "Interact")
	void TurnOnHover();
	UFUNCTION(Category = "Interact")
	void TurnOffHover();
	UFUNCTION(Category = "Interact")
	void TurnOnToggleFunction();

	UFUNCTION()
	void DestroyThisComponentFunc();
	UFUNCTION()
	void SetOverlayMaterial();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};

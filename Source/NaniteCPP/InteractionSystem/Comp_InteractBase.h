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



	UPROPERTY(EditDefaultsOnly, Category="Interact")
	bool IsSlowHoverEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	bool SlowHoverTickEvent;
	UPROPERTY(EditDefaultsOnly, Category="Interact")
	TArray<UMaterialInstanceDynamic*> DMI_List;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* OverlayMaterial;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* OwnerStatic;


public:	


	// Sets default values for this component's properties
	UComp_InteractBase();

	UFUNCTION(Category = "Interact")
	void SetDynamicMaterial(UStaticMeshComponent* Mesh, bool IsSlow);
	UFUNCTION(Category = "Interact")
	void TurnOnHover();
	UFUNCTION(Category = "Interact")
	void TurnOffHover();
	UFUNCTION(Category = "Interact")
	void TurnOnToggleFunction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

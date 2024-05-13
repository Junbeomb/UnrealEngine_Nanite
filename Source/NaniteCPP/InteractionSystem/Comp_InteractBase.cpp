// Fill out your copyright notice in the Description page of Project Settings.



#include "Comp_InteractBase.h"
#include "InteractStatue.h"



// Sets default values for this component's properties
UComp_InteractBase::UComp_InteractBase()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UComp_InteractBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerStatic = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	SetOverlayMaterial();
	// ...
}

void UComp_InteractBase::TurnOnHover()
{
	if (OverlayMaterial) {
		OwnerStatic->SetOverlayMaterial(OverlayMaterial);
	}
}

void UComp_InteractBase::TurnOffHover()
{
	OwnerStatic->SetOverlayMaterial(NULL);
}

void UComp_InteractBase::TurnOnToggleFunction()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"),*GetOwner()->GetName());
	IInterface_Interact* OwnerInterface = Cast<IInterface_Interact>(GetOwner());
	OwnerInterface->PressEStart();
}

void UComp_InteractBase::DestroyThisComponentFunc()
{
	TurnOffHover();
	DestroyComponent();
}

void UComp_InteractBase::SetOverlayMaterial()
{
	if (OverlayMaterial) {
		DMIOverlay = UMaterialInstanceDynamic::Create(OverlayMaterial,this);
		DMIOverlay->SetVectorParameterValue("OverlayColor", OverlayColor);
		OverlayMaterial = DMIOverlay;
	}
	
}


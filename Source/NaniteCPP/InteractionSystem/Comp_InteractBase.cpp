// Fill out your copyright notice in the Description page of Project Settings.



#include "Comp_InteractBase.h"
#include "UObject/ConstructorHelpers.h"
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

	OverlayMaterialInstance = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/3_InteractSystem/MI_OverlayMaterial"));
	DMIOverlay = UMaterialInstanceDynamic::Create(OverlayMaterialInstance, this);
}

void UComp_InteractBase::TurnOnHover()
{
	OwnerStatic = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if ( OwnerStatic && DMIOverlay) {
		OwnerStatic->SetOverlayMaterial(DMIOverlay);
	}
}

void UComp_InteractBase::TurnOffHover()
{
	OwnerStatic = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (OwnerStatic) {
		OwnerStatic->SetOverlayMaterial(NULL);
	}
}

void UComp_InteractBase::SetCantToggle(bool ct)
{
	//cantToggle이 true이면 누르지 못함
	cantToggle = ct;
	if (ct) {
		OverlayColor = { 5,0.5,0.5,1 };
	}
	else {
		OverlayColor = { 0.5,0.5,5,1 };
	}

	DMIOverlay->SetVectorParameterValue("OverlayColor", OverlayColor);
}

void UComp_InteractBase::TurnOnToggleFunction()
{
	if (!cantToggle) {
		IInterface_Interact* OwnerInterface = Cast<IInterface_Interact>(GetOwner());
		OwnerInterface->PressEStart();
	}
}

void UComp_InteractBase::DestroyThisComponentFunc()
{
	TurnOffHover();
	DestroyComponent();
}

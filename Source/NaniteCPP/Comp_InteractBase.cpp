// Fill out your copyright notice in the Description page of Project Settings.


#include "Comp_InteractBase.h"



// Sets default values for this component's properties
UComp_InteractBase::UComp_InteractBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UComp_InteractBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UComp_InteractBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UComp_InteractBase::SetDynamicMaterial(UStaticMeshComponent* Mesh, bool IsSlow)
{
	IsSlowHoverEffect = IsSlow;

	int index = 0;
	for (UMaterialInterface* a : Mesh->GetMaterials()) {
		DMI_List.Add(Mesh->CreateDynamicMaterialInstance(index++, a));
	}
}

void UComp_InteractBase::TurnOnHover()
{
	if (IsSlowHoverEffect) {
		SlowHoverTickEvent = true;
	}
	else {
		for (UMaterialInstanceDynamic* a : DMI_List) {
			a->SetScalarParameterValue(TEXT("HoverValue"), 1.f);
		}
	}
}

void UComp_InteractBase::TurnOffHover()
{
	for (UMaterialInstanceDynamic* a : DMI_List) {
		a->SetScalarParameterValue(TEXT("HoverValue"), 0.f);
	}
	SlowHoverTickEvent = false;
}

void UComp_InteractBase::TurnOnToggleFunction()
{
	//Cast<>(GetOwner())
}


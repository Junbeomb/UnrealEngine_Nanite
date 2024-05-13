// Fill out your copyright notice in the Description page of Project Settings.



#include "Comp_InteractBase.h"
#include "UObject/ConstructorHelpers.h"
#include "InteractStatue.h"



// Sets default values for this component's properties
UComp_InteractBase::UComp_InteractBase()
{
	PrimaryComponentTick.bCanEverTick = false;


}


void UComp_InteractBase::SetCantToggle(bool ct)
{
	//cantToggle�� true�̸� ������ ����
	cantToggle = ct;
	if (ct) {
		OverlayColor = { 5,0.5,0.5 };
	}
	else {
		OverlayColor = { 0.5,0.5,5 };
	}
	SetOverlayMaterial();
}

// Called when the game starts
void UComp_InteractBase::BeginPlay()
{
	Super::BeginPlay();

	OverlayMaterialInstance = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/3_InteractSystem/MI_OverlayMaterial"));

	SetOverlayMaterial();
	// ...
}

void UComp_InteractBase::TurnOnHover()
{
	OwnerStatic = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	//�ӽ÷� �Ҵ�
	//���� ����
	OverlayMaterialInstance = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/3_InteractSystem/MI_OverlayMaterial"));

	UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlayMaterialInstance->GetMaterial()->GetName());
	if ( OwnerStatic && OverlayMaterialInstance) {
		OwnerStatic->SetOverlayMaterial(OverlayMaterialInstance);
	}
}

void UComp_InteractBase::TurnOffHover()
{
	UE_LOG(LogTemp, Warning, TEXT("TurnOffHover"));
	OwnerStatic = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if(OwnerStatic)
		OwnerStatic->SetOverlayMaterial(NULL);
}

void UComp_InteractBase::TurnOnToggleFunction()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"),*GetOwner()->GetName());
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

void UComp_InteractBase::SetOverlayMaterial()
{
	if (OverlayMaterialInstance) {
		DMIOverlay = UMaterialInstanceDynamic::Create(OverlayMaterialInstance,this);
		DMIOverlay->SetVectorParameterValue("OverlayColor", OverlayColor);
		OverlayMaterialInstance = DMIOverlay;
	}
	
}


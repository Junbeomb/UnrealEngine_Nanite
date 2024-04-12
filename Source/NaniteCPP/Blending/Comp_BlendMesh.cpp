// Fill out your copyright notice in the Description page of Project Settings.


#include "Comp_BlendMesh.h"
#include "TurnOffDF.h"

// Sets default values for this component's properties
UComp_BlendMesh::UComp_BlendMesh()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ExtentSubtractAmountOneSecond = 400.f;

	SMC = NULL;
	SKC = NULL;
	// ...
}


// Called when the game starts
void UComp_BlendMesh::BeginPlay()
{
	Super::BeginPlay();
	//StartBlend();
	// ...
	
}


// Called every frame
void UComp_BlendMesh::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsTickStart) {
		SumSeconds += DeltaTime;

		if (SumSeconds > (WhichOneIsLongestXYZ / ExtentSubtractAmountOneSecond) + 0.2) { //다 바꿨으면
			//UE_LOG(LogTemp, Warning, TEXT("Finish"));
			IsTickStart = false;
			SumSeconds = 0.0f;

			if (SMC){
				SMC->SetAffectDistanceFieldLighting(false);
			}
			else {
				SKC->SetAffectDistanceFieldLighting(false);
			}

			for (UMaterialInstanceDynamic* a : DMIList) {
				a->SetScalarParameterValue(TEXT("IsChanging?"), 0.0f);
				a->SetScalarParameterValue(TEXT("Subtract"), 0.0f);
				if (IsHighQuality) {
					a->SetScalarParameterValue(TEXT("IsLow?"), 1.0f);
				}
				else {
					a->SetScalarParameterValue(TEXT("IsLow?"), 0.0f);
				}
			}
			
			IsHighQuality = !IsHighQuality;

			IsBlendStart = false;
		}
		else { //머티리얼 바꾸기
			for (UMaterialInstanceDynamic* a : DMIList) {
				a->SetScalarParameterValue(TEXT("Subtract"), SumSeconds * ExtentSubtractAmountOneSecond);
			}
		}
	}

	// ...
}

void UComp_BlendMesh::StartBlend()
{
	IsBlendStart = true;
	
	FVector Origin;
	FVector BoxExtent;
	GetOwner()->GetActorBounds(false, Origin, BoxExtent, false);
	WhichOneIsLongestXYZ = BoxExtent.GetMax();

	if (IsValid(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))) {
		SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	}
	else {
		SKC = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	}

	if (SMC) {
		SMC->SetAffectDistanceFieldLighting(false);
		CreateDMIAndDFOff(SMC, SMC->GetNumMaterials());
	}
	else {
		SKC->SetAffectDistanceFieldLighting(false);
		CreateDMIAndDFOff(SKC, SKC->GetNumMaterials());
	}
}

void UComp_BlendMesh::JustGo()
{
	UE_LOG(LogTemp, Warning, TEXT("JustGo"));
}

void UComp_BlendMesh::CreateDMIAndDFOff(UPrimitiveComponent* UComp, int NumMaterial)
{
	for (int i = 0; i < NumMaterial; ++i) {
		UMaterialInstanceDynamic* TempDMI = UComp->CreateDynamicMaterialInstance(i, UComp->GetMaterial(i));
		TempDMI->SetScalarParameterValue(TEXT("IsChanging?"), 1.0f);
		DMIList.Add(TempDMI);
	}
	
	float tempScale = WhichOneIsLongestXYZ / 100;
	FVector TempLocation = GetOwner()->GetActorLocation();
	FTransform TempTransform = { {TempLocation.X,TempLocation.Y,TempLocation.Z}, TempLocation,{0,0,0},{tempScale,tempScale,tempScale} };
	GetWorld()->SpawnActor<ATurnOffDF>(ATurnOffDF::StaticClass(), TempTransform);

	IsTickStart = true;
}


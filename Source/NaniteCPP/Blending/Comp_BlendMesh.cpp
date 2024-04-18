// Fill out your copyright notice in the Description page of Project Settings.


#include "Comp_BlendMesh.h"
#include "Kismet/GameplayStatics.h"
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

	Player = Cast<ANaniteCPPCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// ...
}


// Called when the game starts
void UComp_BlendMesh::BeginPlay()
{
	Super::BeginPlay();

	StaticOrSkeletal();
}


// Called every frame
void UComp_BlendMesh::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsTickStart) {
		SumSeconds += DeltaTime;

		if (SumSeconds > (WhichOneIsLongestXYZ / ExtentSubtractAmountOneSecond) + 0.2) { //�� �ٲ�����
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
				if (IsLow()) {
					a->SetScalarParameterValue(TEXT("IsLow?"), 0.0f);
				}
				else {
					a->SetScalarParameterValue(TEXT("IsLow?"), 1.0f);
				}
			}
			
			IsHighQuality = !IsHighQuality;

			IsBlendStart = false;

			if (Player) {
				UE_LOG(LogTemp, Warning, TEXT("UpScore!!"));
				Player->UpScore();
			}

			//
			D_FinishBlending.Broadcast();
		}
		else { //��Ƽ���� �ٲٱ�
			for (UMaterialInstanceDynamic* a : DMIList) {
				a->SetScalarParameterValue(TEXT("Subtract"), SumSeconds * ExtentSubtractAmountOneSecond);
			}
		}
	}
	else if(IsMassTickStart){
		for (UMaterialInstanceDynamic* a : DMIList) {
			a->SetScalarParameterValue(TEXT("Subtract"), SumSeconds * ExtentSubtractAmountOneSecond);
		}
	}

	// ...
}

void UComp_BlendMesh::StartBlend()
{
	bool IsLowObject = IsLow();

	
	if (Player && Player->HighQualityGun != IsLowObject) {
		//UE_LOG(LogTemp, Warning, TEXT("%s"),*Player->GetName());
		D_FinishBlending.Broadcast();
		return;
	}

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

UMeshComponent* UComp_BlendMesh::StaticOrSkeletal()
{
	UMeshComponent* TempMesh;
	if (IsValid(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))) {
		//UE_LOG(LogTemp, Warning, TEXT("Static"));
		OwnerIsStatic = true;
		TempMesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		return TempMesh;
	}
	else if(IsValid(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()))){
		//UE_LOG(LogTemp, Warning, TEXT("Skeletal"));
		OwnerIsStatic = false;
		TempMesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		return TempMesh;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Static or Skeletal Both Nothing"));
	return NULL;
}

bool UComp_BlendMesh::IsLow()
{
	UMeshComponent* TempMesh = StaticOrSkeletal();
	float tempCheck;
	TempMesh->CreateDynamicMaterialInstance(0, TempMesh->GetMaterial(0))->GetScalarParameterValue(TEXT("IsLow?"),tempCheck);

	return tempCheck > 0.5;
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


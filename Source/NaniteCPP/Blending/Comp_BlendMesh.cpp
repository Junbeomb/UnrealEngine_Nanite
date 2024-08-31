
#include "Comp_BlendMesh.h"
#include "Kismet/GameplayStatics.h"
#include "TurnOffDF.h"

UComp_BlendMesh::UComp_BlendMesh()
{
	PrimaryComponentTick.bCanEverTick = false;
	ExtentSubtractAmountOneSecond = 100.f;

	SMC = NULL;
	SKC = NULL;

	Player = Cast<ANaniteCPPCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}


void UComp_BlendMesh::BeginPlay()
{
	Super::BeginPlay();

	StaticOrSkeletal();
}


void UComp_BlendMesh::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SumSeconds += DeltaTime;

	//머티리얼 변환 완료
	if (SumSeconds > (WhichOneIsLongestXYZ / ExtentSubtractAmountOneSecond) + 0.2) { 
		FinishBlendSetVariable();
		D_FinishBlending.Execute();
		return;
	}

	//머티리얼 변환
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetScalarParameterValue(TEXT("Subtract"), SumSeconds * ExtentSubtractAmountOneSecond);
	}
	
}

void UComp_BlendMesh::StartBlend()
{
	bool IsLowObject = IsLow();
	

	if (Player && Player->HighQualityGun != IsLowObject) {
		D_FinishBlending.Execute();
		return;
	}

	IsBlendStart = true;
	
	FVector Origin;
	FVector BoxExtent;
	GetOwner()->GetActorBounds(false, Origin, BoxExtent, false);
	WhichOneIsLongestXYZ = BoxExtent.GetMax();

	UMeshComponent* TempMesh = StaticOrSkeletal();
	if (OwnerIsStatic) {
		SMC = Cast<UStaticMeshComponent>(TempMesh);
		SMC->SetAffectDistanceFieldLighting(false);
		CreateDMIAndDFOff(SMC, SMC->GetNumMaterials());
	}
	else {
		SKC = Cast<USkeletalMeshComponent>(TempMesh);
		SKC->SetAffectDistanceFieldLighting(false);
		CreateDMIAndDFOff(SKC, SKC->GetNumMaterials());
	}

}

void UComp_BlendMesh::JustGo()
{
	D_JustGo.Execute();
}

UMeshComponent* UComp_BlendMesh::StaticOrSkeletal()
{
	UMeshComponent* TempMesh;
	if (IsValid(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))) {
		OwnerIsStatic = true;
		TempMesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		return TempMesh;
	}
	else if(IsValid(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()))){
		OwnerIsStatic = false;
		TempMesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		return TempMesh;
	}
	
	return NULL;
}

bool UComp_BlendMesh::IsLow()
{
	UMeshComponent* TempMesh = StaticOrSkeletal();
	float tempCheck;
	TempMesh->CreateDynamicMaterialInstance(0, TempMesh->GetMaterial(0))->GetScalarParameterValue(TEXT("IsLow?"),tempCheck);

	return tempCheck > 0.5;
}

bool UComp_BlendMesh::GetIsBlendStart()
{
	return IsBlendStart;
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

	PrimaryComponentTick.bCanEverTick = true;
}


void UComp_BlendMesh::FinishBlendSetVariable()
{
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetScalarParameterValue(TEXT("IsChanging?"), 0.0f);
		a->SetScalarParameterValue(TEXT("Subtract"), 0.0f);
		a->SetScalarParameterValue(TEXT("IsLow?"), 1- IsLow());
	}
	SumSeconds = 0.0f;
	IsHighQuality = !IsHighQuality;
}


#include "BlackholeCompBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

UBlackholeCompBase::UBlackholeCompBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	PullStrength = 120.f;
	LinearDampingAmount = 6.f;
	SmallScale = 0.1f;

	InitialNSVortexForceAmount = 1000;
	InitialNSSpawnRate = 7000;

	DecreaseTime = 0.0f;
	DecreaseTimeTotal = 3.0f; //3�ʰ� �پ���

	DecreaseSpawnRateTime = 0.0f;
	DecreaseSpawnRateTimeTotal = 3.0f; //3�ʰ� �پ���

}

void UBlackholeCompBase::BeginPlay()
{
	Super::BeginPlay();

	SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	InitialMaxScale = SMC->GetRelativeScale3D();
}


void UBlackholeCompBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!SMC) return;
	if (!GetIsPull()) return;

	PullDirection = DirectBH(); //���Ϳ� ��Ȧ ���� ���� �ֽ�ȭ

	//SetDFStartDistance
	//�� Tick���� �ٲ�Ƿ� ��� ����
	if (IsValid(Blackhole)) {
		DFStartDistance = Blackhole->GetDFStartRadius();
	}
	//SetDFMask
	for (UMaterialInstanceDynamic* a : DMIList) {
		//�Ÿ� �ִ� 200 ��ŭ ���ֱ�
		a->SetScalarParameterValue(TEXT("DFMaskStartDistance"), DFStartDistance);
		a->SetScalarParameterValue(TEXT("DFMaskFalloff"), DFStartDistance / 3);
	}


	if (IsShrink && NiagaraComp) {
		SMC->SetWorldLocation(PullTargetLocation);
		//Niagara Particle ����
		//VortexForce
		DecreaseTime += DeltaTime;
		if (DecreaseTime <= DecreaseTimeTotal) {
			float VortexAmount = InitialNSVortexForceAmount - ((DecreaseTime / DecreaseTimeTotal) * InitialNSVortexForceAmount);
			VortexAmount = UKismetMathLibrary::MapRangeClamped(VortexAmount, 0, InitialNSVortexForceAmount, 0, InitialNSVortexForceAmount);
			NiagaraComp->SetNiagaraVariableFloat(TEXT("Vortex Force Amount"), VortexAmount);
		}
		else {
			//SpawnRate
			DecreaseSpawnRateTime += DeltaTime;
			if (DecreaseSpawnRateTime <= DecreaseSpawnRateTimeTotal) {
				float SpawnAmount = InitialNSSpawnRate - ((DecreaseSpawnRateTime / DecreaseSpawnRateTimeTotal) * InitialNSSpawnRate);
				SpawnAmount = UKismetMathLibrary::MapRangeClamped(SpawnAmount, 0, InitialNSSpawnRate, 0, InitialNSSpawnRate);
				NiagaraComp->SetNiagaraVariableFloat(TEXT("SpawnRate"), SpawnAmount);
			}
			else {
				//���� ����
				GetOwner()->Destroy();
			}
		}

	}
	else {
		//��Ȧ ���� �Ÿ� 0~1 �� ���� �� ����
		DistanceNormalized = UKismetMathLibrary::MapRangeClamped(PullDirection.Length(), 0, PullStartDistanceToBlackhole, 0, 1);
		//���Ƶ��̱� (AddForce�ϸ� pivot�� �������� ����. ���� DirectBH���� GetOwner()->GetActorLocation() �� �ϸ� �ȴ�)
		FVector ForceValue = PullDirection * (DistanceNormalized / 5 + (1 - DistanceNormalized)) * PullStrength * DeltaTime;
		SMC->AddForce(ForceValue, "None", true);
		SMC->SetLinearDamping(DistanceNormalized * LinearDampingAmount);

		float TempScale = UKismetMathLibrary::MapRangeClamped(DistanceNormalized, 0, 1, SmallScale, InitialMaxScale.X);
		SMC->SetWorldScale3D({ TempScale,TempScale,TempScale });

		//��Ȧ�� ��������� Shrink On
		if (IsDistanceToBH(30.f)) {
			SMC->SetHiddenInGame(true);
			IsShrink = true;
		}
	}

	//Niagara ������Ʈ
	if (IsLateNiagaraSpawnToggle && IsDistanceToBH(DFStartDistance) || !IsLateNiagaraSpawnToggle) {
		if (IsVisibleNiagara) {
			NiagaraComp->SetNiagaraVariablePosition(TEXT("Attractor Position Offset"), PullDirection);
			NiagaraComp->SetNiagaraVariableFloat(TEXT("DFMask Start Distance"), DFStartDistance);
		}
		else {
			NiagaraComp = Cast<UNiagaraComponent>(GetOwner()->GetComponentByClass(UNiagaraComponent::StaticClass()));
			NiagaraComp->SetVisibility(true);
			NiagaraComp->SetHiddenInGame(false);
			NiagaraComp->Activate(true);

			NiagaraComp->SetNiagaraVariablePosition(TEXT("Vortex Axis"), DirectBH());
			NiagaraComp->SetNiagaraVariableFloat(TEXT("Vortex Force Amount"), InitialNSVortexForceAmount);
			NiagaraComp->SetNiagaraVariableFloat(TEXT("SpawnRate"), InitialNSSpawnRate);

			IsVisibleNiagara = true;
		}
	}

}

void UBlackholeCompBase::SetPullOn(ABlackhole* BH, FVector BHLocation)
{
	if (!SMC) return;

	//��Ȧ ����
	Blackhole = BH;
	//��Ȧ ��ġ ����
	SetPullTargetLocation(BHLocation);

	SetPullStartDistance();
	SetInitialNSSpawnRate();
	SMC->SetSimulatePhysics(true);
	SMC->SetEnableGravity(false);
	SMC->SetAngularDamping(100.f);
	SMC->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SMC->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	InitialRotationDegree.Add(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));

	//��Ƽ���� ���̳��� ��Ƽ����� �ٲ㼭 ����
	int numMat = SMC->GetNumMaterials();
	for (int i = 0; i < numMat; ++i) {
		DMIList.Add(SMC->CreateDynamicMaterialInstance(i, SMC->GetMaterial(i)));
	}

	//��Ƽ���� ������� �ϱ�
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetScalarParameterValue(TEXT("DistanceDissolveToggle"), 1.f);
	}

	IsPull = true;
	//IsShrink = false;
}

void UBlackholeCompBase::SetInitialNSSpawnRate()
{
	//��ü�� ũ�⿡ ���� ��ƼŬ ���� ���� + �����ӿ� ���� �ٸ��� ��������
	FVector Origin;
	FVector BoxExtent;
	GetOwner()->GetActorBounds(false, Origin, BoxExtent, false);
	float TempVolume = BoxExtent.X * BoxExtent.Y * BoxExtent.Z;
	TempVolume = FMath::Clamp(TempVolume, 0.f, 3.f);
	InitialNSSpawnRate = InitialNSSpawnRate* (1 / GetWorld()->GetDeltaSeconds()) / 120 * TempVolume;
}

//��Ȧ���� �Ÿ�,����
FVector UBlackholeCompBase::DirectBH() 
{
	return PullTargetLocation - (SMC->GetComponentLocation());
}


void UBlackholeCompBase::SetDFOff(bool IsOn)
{
	if (!SMC) return;
	SMC->SetAffectDistanceFieldLighting(IsOn);
}
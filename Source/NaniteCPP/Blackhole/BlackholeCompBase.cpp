#include "BlackholeCompBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"



UBlackholeCompBase::UBlackholeCompBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	PullStrength = 150.f;
	LinearDampingAmount = 0.8f;
	SmallScale = 0.1f;

	InitialNSVortexForceAmount = 1000;
	InitialNSSpawnRate = 7000;

	DecreaseTime = 0.0f;
	DecreaseTimeTotal = 3.0f; //3초간 줄어들기

	DecreaseSpawnRateTime = 0.0f;
	DecreaseSpawnRateTimeTotal = 3.0f; //3초간 줄어들기

}

void UBlackholeCompBase::BeginPlay()
{
	Super::BeginPlay();

	SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}


void UBlackholeCompBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!SMC) return;

	if (GetIsPull()) {
		PullDirection = DirectBH(); //액터와 블랙홀 간의 방향 최신화
		//주인 액터의 staticmeshcomponent 얻어오기

		if (IsShrink) {
			SMC->SetWorldLocation(PullTargetLocation);

			//Niagara Particle 감소
			if (NiagaraComp) {
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
						//UE_LOG(LogTemp, Warning, TEXT("%f"), SpawnAmount);
						NiagaraComp->SetNiagaraVariableFloat(TEXT("SpawnRate"), SpawnAmount);
					}
					else {
						//Destroy
						//UE_LOG(LogTemp, Warning, TEXT("Destroy"));
						GetOwner()->Destroy();
					}
				}
			}

		}
		else {
			//블랙홀 과의 거리 0~1 로 만든 값 저장
			DistanceNormalized = UKismetMathLibrary::MapRangeClamped(PullDirection.Length(), 0, PullStartDistanceToBlackhole, 0, 1);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *SMC->GetComponentLocation().ToString());
			//UE_LOG(LogTemp, Warning, TEXT("Shrink false"));
			//빨아들이기 (AddForce하면 pivot은 움직이지 않음. 따라서 DirectBH에서 GetOwner()->GetActorLocation() 을 하면 안댐)
			FVector ForceValue = PullDirection * (DistanceNormalized / 5 + (1 - DistanceNormalized)) * PullStrength * DeltaTime;
			SMC->AddForce(ForceValue, "None", true);

			SMC->SetLinearDamping(DistanceNormalized * LinearDampingAmount);

			float TempScale = UKismetMathLibrary::MapRangeClamped(DistanceNormalized, 0, 1, SmallScale, InitialMaxScale.X);
			SMC->SetWorldScale3D({ TempScale,TempScale,TempScale });

			//블랙홀가 가까워지면 Shrink On
			if (IsDistanceToBH(30.f)) {
				SMC->SetHiddenInGame(true);
				IsShrink = true;
			}
		}

		//SetNiagara
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

		//SetDFStartDistance
		if (IsValid(Blackhole)) {
			DFStartDistance = Blackhole->DFStartRadius;
			//UE_LOG(LogTemp, Warning, TEXT("%f"), Blackhole->DFStartRadius);
		}

		//SetDFMask
		for (UMaterialInstanceDynamic* a : DMIList) {
			
			//거리 최대 200 만큼 빼주기
			a->SetScalarParameterValue(TEXT("DFMaskStartDistance"), DFStartDistance);
			//최대 대략 70 
			a->SetScalarParameterValue(TEXT("DFMaskFalloff"), DFStartDistance/3);
		}

		//WillDie
		if (IsDistanceToBH(DFStartDistance)) {
			IsWillDie = true;
		}
	}
}

void UBlackholeCompBase::SetPullOn(ABlackhole* BH, FVector BHLocation)
{
	if (!SMC) return;

	//UE_LOG(LogTemp, Warning, TEXT("%f"), BHLocation.X);

	//블랙홀 저장
	Blackhole = BH;
	//블랙홀 위치 저장
	SetPullTargetLocation(BHLocation);

	SetPullStartDistance();
	SetInitialMaxScale();
	SetInitialNSSpawnRate();
	SMC->SetSimulatePhysics(true);
	SMC->SetEnableGravity(false);
	SMC->SetAngularDamping(100.f);
	SMC->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SMC->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	InitialRotationDegree.Add(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));

	//머티리얼 다이나믹 머티리얼로 바꿔서 저장
	int numMat = SMC->GetNumMaterials();
	for (int i = 0; i < numMat; ++i) {
		DMIList.Add(SMC->CreateDynamicMaterialInstance(i, SMC->GetMaterial(i)));
	}

	//**************************
	//**************************
	//블랙홀 Wind 감소 부분 추가하기
	//**************************
	//**************************

	//머티리얼 사라지게 하기
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetScalarParameterValue(TEXT("DistanceDissolveToggle"), 1.f);
	}

	IsPull = true;
	//IsShrink = false;
}

void UBlackholeCompBase::SetInitialNSSpawnRate()
{

	FVector Origin;
	FVector BoxExtent;
	GetOwner()->GetActorBounds(false, Origin, BoxExtent, false);
	float TempVolume = BoxExtent.X * BoxExtent.Y * BoxExtent.Z;
	TempVolume = FMath::Clamp(TempVolume, 0.f, 3.f);
	InitialNSSpawnRate = InitialNSSpawnRate* (1 / GetWorld()->GetDeltaSeconds()) / 120 * TempVolume;

	UE_LOG(LogTemp, Warning, TEXT("%f"), InitialNSSpawnRate);
}

FVector UBlackholeCompBase::DirectBH() {
	//블랙홀과의 거리,방향
	return PullTargetLocation - (SMC->GetComponentLocation());
}


void UBlackholeCompBase::SetDFOff(bool IsOn)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Blackhole->DFStartRadius);
	if (!SMC) return;

	SMC->SetAffectDistanceFieldLighting(IsOn);
}
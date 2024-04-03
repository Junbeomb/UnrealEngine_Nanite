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
	InitialNSSpawnRate = 20000;
}


// Called when the game starts
void UBlackholeCompBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void UBlackholeCompBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (GetIsPull()) {
		PullDirection = DirectBH(); //���Ϳ� ��Ȧ ���� ���� �ֽ�ȭ
		//���� ������ staticmeshcomponent ������
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if (IsShrink) {
			SMC->SetWorldLocation(PullTargetLocation);
			//�ӽ� Destroy
			GetOwner()->Destroy();
		}
		else {
			//��Ȧ ���� �Ÿ� 0~1 �� ���� �� ����
			DistanceNormalized = UKismetMathLibrary::MapRangeClamped(PullDirection.Length(), 0, PullStartDistanceToBlackhole, 0, 1);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *SMC->GetComponentLocation().ToString());

			//���Ƶ��̱� (AddForce�ϸ� pivot�� �������� ����. ���� DirectBH���� GetOwner()->GetActorLocation() �� �ϸ� �ȴ�)
			FVector ForceValue = PullDirection * (DistanceNormalized / 5 + (1 - DistanceNormalized)) * PullStrength * DeltaTime;
			SMC->AddForce(ForceValue, "None", true);

			SMC->SetLinearDamping(DistanceNormalized * LinearDampingAmount);

			float TempScale = UKismetMathLibrary::MapRangeClamped(DistanceNormalized, 0, 1, SmallScale, InitialMaxScale.X);
			SMC->SetWorldScale3D({ TempScale,TempScale,TempScale });

			//��Ȧ�� ��������� Shrink On
			if (IsDistanceToBH(30.f)) {
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
		}

		//SetDFMask
		for (UMaterialInstanceDynamic* a : DMIList) {
			a->SetScalarParameterValue("DFMaskStartDistance", DFStartDistance - 30);
		}

		//WillDie
		if (IsDistanceToBH(DFStartDistance)) {
			IsWillDie = true;
		}
	}
}

void UBlackholeCompBase::SetPullOn(ABlackhole* BH, FVector BHLocation)
{

	//UE_LOG(LogTemp, Warning, TEXT("%f"), BHLocation.X);

	//��Ȧ ����
	Blackhole = BH;
	//��Ȧ ��ġ ����
	SetPullTargetLocation(BHLocation);

	SetPullStartDistance();
	SetInitialMaxScale();
	UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
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

	//**************************
	//**************************
	//��Ȧ Wind ���� �κ� �߰��ϱ�
	//**************************
	//**************************

	//��Ƽ���� ������� �ϱ�
	for (UMaterialInstanceDynamic* a : DMIList) {
		a->SetScalarParameterValue("DistanceDissolveToggle", 1.f);
	}

	IsPull = true;
	IsShrink = false;
}
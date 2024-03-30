#include "BlackholeCompBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"


UBlackholeCompBase::UBlackholeCompBase()
{

	PrimaryComponentTick.bCanEverTick = true;


	PullStrength = 150.f;
	LinearDampingAmount = 0.8f;
	SmallScale = 0.1f;
}


// Called when the game starts
void UBlackholeCompBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void UBlackholeCompBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetDeltaTime(DeltaTime);


	if (GetIsPull()) {
		PullDirection = DirectBH(); //���Ϳ� ��Ȧ ���� ���� �ֽ�ȭ
		//���� ������ staticmeshcomponent ������
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if (GetIsShrink()) {
			SMC->SetWorldLocation(PullTargetLocation);

			//�ӽ� Destroy
			GetOwner()->Destroy();
		}
		else {
			//��Ȧ ���� �Ÿ� 0~1 �� ���� �� ����
			DistanceNormalized = UKismetMathLibrary::MapRangeClamped(PullDirection.Length(), 0, PullStartDistanceToBlackhole, 0, 1);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *SMC->GetComponentLocation().ToString());

			//���Ƶ��̱� (AddForce�ϸ� pivot�� �������� ����. ���� DirectBH���� GetOwner()->GetActorLocation() �� �ϸ� �ȴ�)
			FVector ForceValue = PullDirection * (DistanceNormalized / 5 + (1 - DistanceNormalized)) * PullStrength * GetDeltaTime();
			SMC->AddForce(ForceValue, "None", true);

			SMC->SetLinearDamping(DistanceNormalized * LinearDampingAmount);

			float TempScale = UKismetMathLibrary::MapRangeClamped(DistanceNormalized, 0, 1, SmallScale, InitialMaxScale.X);
			SMC->SetWorldScale3D({ TempScale,TempScale,TempScale });

			//��Ȧ�� ��������� Shrink On
			if (PullDirection.Length() <= 30.f) {
				SetIsShrink(true);
			}
		}
	}
}

void UBlackholeCompBase::SetPullOn(ABlackhole* BH, FVector BHLocation)
{

	//UE_LOG(LogTemp, Warning, TEXT("%f"), BHLocation.X);

	//��Ȧ ����
	SetBlackhole(BH);
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

	SetIsPull(true);
	SetIsShrink(false);
}
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
		PullDirection = DirectBH(); //액터와 블랙홀 간의 방향 최신화
		//주인 액터의 staticmeshcomponent 얻어오기
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if (GetIsShrink()) {
			SMC->SetWorldLocation(PullTargetLocation);

			//임시 Destroy
			GetOwner()->Destroy();
		}
		else {
			//블랙홀 과의 거리 0~1 로 만든 값 저장
			DistanceNormalized = UKismetMathLibrary::MapRangeClamped(PullDirection.Length(), 0, PullStartDistanceToBlackhole, 0, 1);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *SMC->GetComponentLocation().ToString());

			//빨아들이기 (AddForce하면 pivot은 움직이지 않음. 따라서 DirectBH에서 GetOwner()->GetActorLocation() 을 하면 안댐)
			FVector ForceValue = PullDirection * (DistanceNormalized / 5 + (1 - DistanceNormalized)) * PullStrength * GetDeltaTime();
			SMC->AddForce(ForceValue, "None", true);

			SMC->SetLinearDamping(DistanceNormalized * LinearDampingAmount);

			float TempScale = UKismetMathLibrary::MapRangeClamped(DistanceNormalized, 0, 1, SmallScale, InitialMaxScale.X);
			SMC->SetWorldScale3D({ TempScale,TempScale,TempScale });

			//블랙홀가 가까워지면 Shrink On
			if (PullDirection.Length() <= 30.f) {
				SetIsShrink(true);
			}
		}
	}
}

void UBlackholeCompBase::SetPullOn(ABlackhole* BH, FVector BHLocation)
{

	//UE_LOG(LogTemp, Warning, TEXT("%f"), BHLocation.X);

	//블랙홀 저장
	SetBlackhole(BH);
	//블랙홀 위치 저장
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
		a->SetScalarParameterValue("DistanceDissolveToggle", 1.f);
	}

	SetIsPull(true);
	SetIsShrink(false);
}
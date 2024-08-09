
#include "FoliagePlantBase.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedFoliageActor.h"
#include "../Blending/Comp_BlendMesh.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Containers/UnrealString.h"
#include "FoliageInfluencer.h"
#include "NiagaraComponent.h"

AFoliagePlantBase::AFoliagePlantBase()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlappingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlappingSphere"));
	OverlappingSphere->SetupAttachment(MeshComponent);
	OverlappingSphere->SetSphereRadius(32.f);

	OverlappingCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlappingCapsule"));
	OverlappingCapsule->SetupAttachment(MeshComponent);
	OverlappingCapsule->SetCapsuleHalfHeight(44.f);
	OverlappingCapsule->SetCapsuleRadius(22.f);

	SoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEffect"));

	//pullRange�� ��ġ��
	OverlappingCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFoliagePlantBase::OverlapSphereOrCapsule);

	overlapIsSphere = false;
	soundOn = false;

	//������ ��ȯ����.
	IsSlowFoliageReturn = false;
	TimelineFinishDoOnce = false;

	//Fast Timeline ����
	FastTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("FastTimeline"));
	//Slow Timeline ����
	SlowTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("SlowTimeline"));
	//callback�Լ�bind
	floatTimelineCallback.BindUFunction(this, FName("BlendWeightTimelineUpdate"));
	floatTimelineFinishedCallback.BindUFunction(this, FName("BlendWeightTimelineFinish"));

}


void AFoliagePlantBase::ReturnToFoliage() 
{
	Super::ReturnToFoliage();

	//sphere�� �� ���� capsule�� ������
	if (overlapIsSphere) {
		OverlappingCapsule->DestroyComponent();
	}
	else {
		OverlappingSphere->DestroyComponent();
	}

	if (!soundOn) {
		SoundEffect->DestroyComponent();
	}
}

void AFoliagePlantBase::checkToFoliageInfluencer() {

	Super::checkToFoliageInfluencer();

	if (IsInfluencersInRange) { //������ Influencers�� �ϳ��� ������.
		FastTimeline->Stop();
		SlowTimeline->Stop();
		MeshComponent->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("b_Base")), 1.0f, false, true);
		DoOnce = false;
	}
}

//������ InfluencersInRange�� �ƹ��͵� ������ �����ϴ� �Լ�.
void AFoliagePlantBase::NoInfluencersInRangeFunc()
{
	if (!IsSlowFoliageReturn) {
		FastTimeline->AddInterpFloat(FastCurve, floatTimelineCallback, FName("FastBlend"));
		FastTimeline->SetTimelineFinishedFunc(floatTimelineFinishedCallback);

		FastTimeline->PlayFromStart();
	}
	else {
		SlowTimeline->AddInterpFloat(FastCurve, floatTimelineCallback, FName("SlowBlend"));
		SlowTimeline->SetTimelineFinishedFunc(floatTimelineFinishedCallback);

		SlowTimeline->PlayFromStart();
	}
}

//Timeline ������Ʈ��
void AFoliagePlantBase::BlendWeightTimelineUpdate(float Value)
{
	PhysicsAlpha = Value;
	MeshComponent->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("b_Base")), PhysicsAlpha, false, true);
}

//overlap �Ǹ�
void AFoliagePlantBase::OverlapSphereOrCapsule(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Physics")) {
		//UE_LOG(LogTemp, Warning, TEXT("Physics"));
		MeshComponent->SetAllBodiesBelowSimulatePhysics(FName(TEXT("b_Base")), true, false);
	}
}

//Timeline�� ������
void AFoliagePlantBase::BlendWeightTimelineFinish()
{

	//ũ�⸸ŭ ����ϴٰ� BP�� ��ȯ��Ű��
	//���� �ϼ��Ǹ� ����

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);

	if (!TimelineFinishDoOnce) {
		TimelineFinishDoOnce = true;

		//WorldFoliage�� �ִ� �Ĺ��� ��� �����ͼ� ����.
		//Beginplay���� WorldFoliage�� 0��°�� ���������Ƿ� landscape�� ù��° Foliage���� �����´�.
		//���� OpenWorldó�� Landscape�������� ���ĳ��� �ʿ��� �����ö����� InstancedFoliageActor[0] ��°�� ��� ������ �޽��� �־���Ѵ�.
		TArray<UActorComponent*> Temp;
		WorldFoliage->GetComponents(UInstancedStaticMeshComponent::StaticClass(), Temp);


		int count = 0;
		int classIndex = 0;

		for (UActorComponent* Comp : Temp) {

			UInstancedStaticMeshComponent* InstancedMesh = Cast<UInstancedStaticMeshComponent>(Comp);

			FString LeftS;
			FString RightS;

			if (Comp_Blend->IsLow()) {
				MeshComponent->GetSkeletalMeshAsset()->GetName().Split(TEXT("SK_"), &LeftS, &RightS);
				RightS = "L_" + RightS;

				FString value = *InstancedMesh->GetStaticMesh()->GetName();

				bool isContain = value.Contains(*RightS, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				if (isContain) {
					classIndex = count;
				}
				++count;
			}
			else {
				MeshComponent->GetSkeletalMeshAsset()->GetName().Split(TEXT("SK_"), &LeftS, &RightS);
				RightS = "H_" + RightS;

				FString value = *InstancedMesh->GetStaticMesh()->GetName();

				bool isContain = value.Contains(*RightS, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				if (isContain) {
					classIndex = count;
				}
				++count;
			}

		}

		Cast<UInstancedStaticMeshComponent>(Temp[classIndex])->AddInstance(MeshComponent->GetComponentTransform(), true);
		Destroy();
	}
}


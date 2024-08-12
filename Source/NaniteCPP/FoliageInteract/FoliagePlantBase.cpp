
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

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	OverlappingCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlappingCapsule"));
	OverlappingCapsule->SetupAttachment(MeshComponent);
	OverlappingCapsule->SetCapsuleHalfHeight(44.f);
	OverlappingCapsule->SetCapsuleRadius(22.f);
	OverlappingCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFoliagePlantBase::OverlapSphereOrCapsule);

	SoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEffect"));

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
	floatTimelineFinishedCallback.BindUFunction(this, FName("NoInfluencersInRangeFunc"));

}

//������ InfluencersInRange�� �ƹ��͵� ������ �����ϴ� �Լ�.
void AFoliagePlantBase::NoInfluencersInRangeTL()
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
		MeshComponent->SetAllBodiesBelowSimulatePhysics(FName(TEXT("b_Base")), true, false);
	}
}

void AFoliagePlantBase::GetMeshName(FString& LeftS, FString& RightS)
{
	MeshComponent->GetSkeletalMeshAsset()->GetName().Split(TEXT("SK_"), &LeftS, &RightS);
}

void AFoliagePlantBase::SetCollisionSimulate()
{
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);
}

void AFoliagePlantBase::AddFoliageInstance(UActorComponent* AC)
{
	Cast<UInstancedStaticMeshComponent>(AC)->AddInstance(MeshComponent->GetComponentTransform(), true);
}

void AFoliagePlantBase::GoCustomFunc()
{
	NoInfluencersInRangeTL();
}

void AFoliagePlantBase::TurnOffAnim()
{
	FastTimeline->Stop();
	SlowTimeline->Stop();
	MeshComponent->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("b_Base")), 1.0f, false, true);
}

void AFoliagePlantBase::DestroyCompFunc()
{
	OverlappingCapsule->DestroyComponent();
	if (!soundOn) SoundEffect->DestroyComponent();
}


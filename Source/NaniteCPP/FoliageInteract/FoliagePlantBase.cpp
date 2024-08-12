
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

	//느리게 변환안함.
	IsSlowFoliageReturn = false;
	TimelineFinishDoOnce = false;

	//Fast Timeline 생성
	FastTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("FastTimeline"));
	//Slow Timeline 생성
	SlowTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("SlowTimeline"));
	//callback함수bind
	floatTimelineCallback.BindUFunction(this, FName("BlendWeightTimelineUpdate"));
	floatTimelineFinishedCallback.BindUFunction(this, FName("NoInfluencersInRangeFunc"));

}

//주위에 InfluencersInRange가 아무것도 없을때 실행하는 함수.
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

//Timeline 업데이트시
void AFoliagePlantBase::BlendWeightTimelineUpdate(float Value)
{
	PhysicsAlpha = Value;
	MeshComponent->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("b_Base")), PhysicsAlpha, false, true);
}

//overlap 되면
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


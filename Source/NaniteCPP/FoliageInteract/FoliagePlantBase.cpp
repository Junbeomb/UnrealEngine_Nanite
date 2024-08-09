
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

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

	OverlappingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlappingSphere"));
	OverlappingSphere->SetupAttachment(MeshComponent);
	OverlappingSphere->SetSphereRadius(32.f);

	OverlappingCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlappingCapsule"));
	OverlappingCapsule->SetupAttachment(MeshComponent);
	OverlappingCapsule->SetCapsuleHalfHeight(44.f);
	OverlappingCapsule->SetCapsuleRadius(22.f);

	SoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEffect"));

	//pullRange랑 겹치면
	OverlappingCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFoliagePlantBase::OverlapSphereOrCapsule);

	overlapIsSphere = false;
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


void AFoliagePlantBase::ReturnToFoliage() 
{

	//sphere를 쓸 건지 capsule을 쓸건지
	if (overlapIsSphere) {
		OverlappingCapsule->DestroyComponent();
	}
	else {
		OverlappingSphere->DestroyComponent();
	}

	if (!soundOn) {
		SoundEffect->DestroyComponent();
	}

	//월드 내의 모든 FoliageInfluencer를 array 에 담기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFoliageInfluencer::StaticClass(), AllFoliageInfluencers);
	FoliageInfluencersLen = AllFoliageInfluencers.Num();

	//월드 내의 Foliage를 가져와서 첫번째꺼를 WorldFoliage에 담기
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), Temp);
	if (Temp.Num() > 0)
		WorldFoliage = Cast<AInstancedFoliageActor>(Temp[0]);

	//옆에 FoliageInfluencer 있는지 확인하기
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliagePlantBase::checkToFoliageInfluencer, 0.05, false);
}

void AFoliagePlantBase::checkToFoliageInfluencer() {

	//UE_LOG(LogTemp, Warning, TEXT("%d"), FoliageInfluencersLen);
	//자신의 거리 밖에 있는 FoliageInfluencer 빼주기
	for (AActor* a : AllFoliageInfluencers) {
		AFoliageInfluencer* FI = Cast<AFoliageInfluencer>(a);
		float distance = GetDistanceTo(FI);
		float radius = FI->PhysicsRadius + 100;

		if (distance > radius) {
			--FoliageInfluencersLen;
		}
	}

	//만약에 하나라도 거리 안에 있다
	if (FoliageInfluencersLen > 0) {
		FoliageInfluencersLen = AllFoliageInfluencers.Num();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliagePlantBase::checkToFoliageInfluencer, 0.1, false); //Influencers 가 모두 주변에 없을때까지 반복
		IsInfluencersInRange = true;

	}
	else { //하나도 거리안에 Influencers 가 없다.
		IsInfluencersInRange = false;
	}

	if (!IsInfluencersInRange) { // 주위에 Influencers가 없으면
		if (!DoOnce) { //한번만 실행
			DoOnce = true;
			NoInfluencersInRangeTL();
		}
	}
	if (IsInfluencersInRange) { //주위에 Influencers가 하나라도 있으면.
		FastTimeline->Stop();
		SlowTimeline->Stop();
		MeshComponent->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("b_Base")), 1.0f, false, true);
		DoOnce = false;
	}
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
		//UE_LOG(LogTemp, Warning, TEXT("Physics"));
		MeshComponent->SetAllBodiesBelowSimulatePhysics(FName(TEXT("b_Base")), true, false);
	}
}

void AFoliagePlantBase::BeginPlay()
{
	Super::BeginPlay();
	Comp_Blend->D_FinishBlending.BindUObject(this, &AFoliagePlantBase::ReturnToFoliage);
	Comp_Blend->D_JustGo.BindUObject(this, &AFoliagePlantBase::ReturnToFoliage);
}

//Timeline이 끝나면
void AFoliagePlantBase::NoInfluencersInRangeFunc()
{

	//크기만큼 대기하다가 BP로 변환시키기
	//블프 완성되면 ㄱㄱ
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);

	if (!TimelineFinishDoOnce) {
		TimelineFinishDoOnce = true;

		//WorldFoliage에 있는 식물들 모두 가져와서 저장.
		//Beginplay에서 WorldFoliage의 0번째를 가져왔으므로 landscape의 첫번째 Foliage만을 가져온다.
		//따라서 OpenWorld처럼 Landscape여러장을 겹쳐놓은 맵에서 가져올때에는 InstancedFoliageActor[0] 번째에 모든 종류의 메쉬가 있어야한다.
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
			}
			else {
				MeshComponent->GetSkeletalMeshAsset()->GetName().Split(TEXT("SK_"), &LeftS, &RightS);
				RightS = "H_" + RightS;
			}

			FString value = *InstancedMesh->GetStaticMesh()->GetName();

			bool isContain = value.Contains(*RightS, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			if (isContain) {
				classIndex = count;
			}
			++count;
		}

		Cast<UInstancedStaticMeshComponent>(Temp[classIndex])->AddInstance(MeshComponent->GetComponentTransform(), true);
		Destroy();
	}
}


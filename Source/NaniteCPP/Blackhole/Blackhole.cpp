
#include "Blackhole.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Components/SphereComponent.h"
#include "BlackholeCompBase.h"
#include "../FoliageInteract/FoliageInfluencer.h"

ABlackhole::ABlackhole()
{
	PrimaryActorTick.bCanEverTick = false;

	DieToggle = false;


	//루트 컴포넌트 설정 및 매쉬컴포넌트 부착
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BlackholeBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	BlackholeBaseMesh->SetupAttachment(RootComponent);

	FoliageToBPActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("FoliageToBPActor"));
	FoliageToBPActor->SetupAttachment(RootComponent);

	

	TurnOffDFRange = CreateDefaultSubobject<USphereComponent>(TEXT("TurnOffDFRange"));
	TurnOffDFRange->SetupAttachment(RootComponent);
	TurnOffDFRange->SetSphereRadius(0.f);
	TurnOffDFRange->SetHiddenInGame(true);

	PullRange = CreateDefaultSubobject<USphereComponent>(TEXT("PullRange"));
	PullRange->SetupAttachment(RootComponent);
	PullRange->SetSphereRadius(0.f);
	PullRange->SetHiddenInGame(true);

	//블랙홀 매쉬 설정
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/1_Blackhole/SM_BlackholeMesh"));
	BlackholeBaseMesh->SetStaticMesh(MeshAsset.Object);

	//블랙홀 생성 Timeline
		MeshTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("MeshTimeline"));
		//어떤 curve로 쓸거
		ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/1_Blackhole/Curve/FC_BlackholeSizeIncreaseCurve"));
		MeshCurve = Curve.Object;
		//callback함수bind
		floatTimelineCallback.BindUFunction(this,FName("SetScaleTimelineUpdate"));
		floatTimelineFinishedCallback.BindUFunction(this, FName("SetScaleTimelineFinish"));

	//Range Timeline (한 개의 Timeline안에 여러개의 curve 존재)
		RangeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RangeTimeline"));
		//callback함수bind
		RangeTimelineFinishedCallback.BindUFunction(this, FName("RangeTimelineFinish"));
		//어떤 curve로 쓸거
		ConstructorHelpers::FObjectFinder<UCurveFloat> PullCurve(TEXT("/Game/1_Blackhole/Curve/FC_PullRange"));
		PullRangeCurve = PullCurve.Object;
		ConstructorHelpers::FObjectFinder<UCurveFloat> DFCurve(TEXT("/Game/1_Blackhole/Curve/FC_DFRange"));
		DFRangeCurve = DFCurve.Object;
		ConstructorHelpers::FObjectFinder<UCurveFloat> FoliageCurve(TEXT("/Game/1_Blackhole/Curve/FC_FoliageRange"));
		FoliageRangeCurve = FoliageCurve.Object;
		//callback함수bind
		PullRangeTimelineCallback.BindUFunction(this, FName("PullRangeTimelineUpdate"));
		DFRangeTimelineCallback.BindUFunction(this, FName("DFRangeTimelineUpdate"));
		FoliageRangeTimelineCallback.BindUFunction(this, FName("FoliageRangeTimelineUpdate"));

		//pullRange랑 겹치면
		PullRange->OnComponentBeginOverlap.AddDynamic(this, &ABlackhole::OverlapPullRange);
}

void ABlackhole::BeginPlay()
{
	Super::BeginPlay();
	//ScaleTimeline
	if (MeshCurve != nullptr) {
		MeshTimeline->AddInterpFloat(MeshCurve, floatTimelineCallback,FName("MeshScale"));
		MeshTimeline->SetTimelineFinishedFunc(floatTimelineFinishedCallback);
		
		MeshTimeline->PlayFromStart();
	}

	//Die
	DieBlackhole();


	FoliageToBPActor->SetChildActorClass(WhatChildActor);
	//BP_BH 로 바뀌는 거라고 알려주기
	if (FoliageToBPActor->GetChildActor()) {
		AFoliageInfluencer* FoliageInfluencer = Cast<AFoliageInfluencer>(FoliageToBPActor->GetChildActor());
		if(FoliageInfluencer)
			FoliageInfluencer->IsBlackholeInfluencer = true;
	}

	//RangeTimeline 커브가 모두 null이 아닐때
	if (PullRangeCurve && DFRangeCurve && FoliageRangeCurve) {
		RangeTimeline->AddInterpFloat(PullRangeCurve, PullRangeTimelineCallback, FName("PullRange"));
		RangeTimeline->AddInterpFloat(DFRangeCurve, DFRangeTimelineCallback, FName("DFRange"));
		RangeTimeline->AddInterpFloat(FoliageRangeCurve, FoliageRangeTimelineCallback, FName("FoliageRange"));

		RangeTimeline->SetTimelineFinishedFunc(RangeTimelineFinishedCallback);

		RangeTimeline->PlayFromStart();
	}

}

void ABlackhole::SetScaleTimelineUpdate(float Value)
{
	//블랙홀의Mesh 크기가 Timeline에 따라 늘어나거나 줄어듬
	BlackholeBaseMesh->SetWorldScale3D({ Value,Value,Value });
}

void ABlackhole::SetScaleTimelineFinish()
{
	//줄어드는 Timeline이 끝나면
	if (DieToggle) {
		Destroy();
		return;
	}
}

void ABlackhole::PullRangeTimelineUpdate(float Value)
{
	PullRange->SetSphereRadius(Value);
}

void ABlackhole::DFRangeTimelineUpdate(float Value)
{
	TurnOffDFRange->SetSphereRadius(Value);
}

void ABlackhole::FoliageRangeTimelineUpdate(float Value)
{
	AFoliageInfluencer* FoliageInfluencer = Cast<AFoliageInfluencer>(FoliageToBPActor->GetChildActor());
	if(FoliageInfluencer)
		FoliageInfluencer->PhysicsRadius = Value;
}

void ABlackhole::RangeTimelineFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("Range Timeline Finish"));
}


//PullRange에 actor 가 겹치면
void ABlackhole::OverlapPullRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UBlackholeCompBase* BHComp = Cast<UBlackholeCompBase>(OtherActor->GetComponentByClass(UBlackholeCompBase::StaticClass()));
	if (BHComp) {
		if (!BHComp->GetIsPull()) { //해당 물체를 당기고 있지 않을때만
			BHComp->SetPullOn(this, GetActorLocation());
		}
	}

}



void ABlackhole::DieBlackhole() 
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate TimerDelegate;

	//Delay -> 6초뒤 Mesh줄어드는 Animation실행
	TimerDelegate.BindLambda([&]
	{
			DieToggle = true;
			MeshTimeline->SetPlayRate(10.f);
			MeshTimeline->Reverse();
	});

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, 6, false);

}



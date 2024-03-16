
#include "Blackhole.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Components/SphereComponent.h"

ABlackhole::ABlackhole()
{
	PrimaryActorTick.bCanEverTick = false;

	DieToggle = false;


	//루트 컴포넌트 설정 및 매쉬컴포넌트 부착
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BlackholeBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	BlackholeBaseMesh->SetupAttachment(RootComponent);

	TurnOffDFRange = CreateDefaultSubobject<USphereComponent>(TEXT("TurnOffDFRange"));
	TurnOffDFRange->SetupAttachment(RootComponent);
	TurnOffDFRange->SetSphereRadius(0.f);
	TurnOffDFRange->SetHiddenInGame(false);

	PullRange = CreateDefaultSubobject<USphereComponent>(TEXT("PullRange"));
	PullRange->SetupAttachment(RootComponent);
	PullRange->SetSphereRadius(0.f);
	PullRange->SetHiddenInGame(false);

	//블랙홀 매쉬 설정
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Blackhole/SM_BlackholeMesh"));
	BlackholeBaseMesh->SetStaticMesh(MeshAsset.Object);

	//블랙홀 생성 Timeline
		MeshTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("MeshTimeline"));
		//어떤 curve로 쓸거
		ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blackhole/Curve/FC_BlackholeSizeIncreaseCurve"));
		MeshCurve = Curve.Object;
		//callback함수bind
		floatTimelineCallback.BindUFunction(this,FName("SetScaleTimelineUpdate"));
		floatTimelineFinishedCallback.BindUFunction(this, FName("SetScaleTimelineFinish"));

	//Range Timeline
		RangeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RangeTimeline"));
		//callback함수bind
		RangeTimelineFinishedCallback.BindUFunction(this, FName("RangeTimelineFinish"));
		//어떤 curve로 쓸거
		ConstructorHelpers::FObjectFinder<UCurveFloat> PullCurve(TEXT("/Game/Blackhole/Curve/FC_PullRange"));
		PullRangeCurve = PullCurve.Object;
		ConstructorHelpers::FObjectFinder<UCurveFloat> DFCurve(TEXT("/Game/Blackhole/Curve/FC_DFRange"));
		DFRangeCurve = DFCurve.Object;
		//callback함수bind
		PullRangeTimelineCallback.BindUFunction(this, FName("PullRangeTimelineUpdate"));
		DFRangeTimelineCallback.BindUFunction(this, FName("DFRangeTimelineUpdate"));


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

	//RangeTimeline 커브가 모두 null이 아닐때
	if (PullRangeCurve && DFRangeCurve) {
		RangeTimeline->AddInterpFloat(PullRangeCurve, PullRangeTimelineCallback, FName("PullRange"));
		RangeTimeline->AddInterpFloat(DFRangeCurve, DFRangeTimelineCallback, FName("DFRange"));

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

void ABlackhole::RangeTimelineFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("Range Timeline Finish"));
}



void ABlackhole::DieBlackhole() 
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate TimerDelegate;

	//Delay -> 6초뒤 Mesh줄어드는 Animation실행
	TimerDelegate.BindLambda([&]
	{
			DieToggle = true;
			MeshTimeline->Reverse();
	});

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, 6, false);

}



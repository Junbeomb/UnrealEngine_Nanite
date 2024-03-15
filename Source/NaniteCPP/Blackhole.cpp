
#include "Blackhole.h"


ABlackhole::ABlackhole()
{
	PrimaryActorTick.bCanEverTick = true;

	//루트 컴포넌트 설정 및 매쉬컴포넌트 부착
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BlackholeBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	BlackholeBaseMesh->SetupAttachment(RootComponent);

	//블랙홀 매쉬 설정
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Blackhole/SM_BlackholeMesh"));
	BlackholeBaseMesh->SetStaticMesh(MeshAsset.Object);


	//블랙홀 생성 Timeline
	MeshTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("Timeline"));
		//어떤 curve로 쓸거
	ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blackhole/FC_BlackholeSizeIncreaseCurve"));
	FloatCurve = Curve.Object;
	TimelineLength = 3.f;
		//callback함수bind
	floatTimelineCallback.BindUFunction(this,FName("SetScaleTimelineUpdate"));
	floatTimelineFinishedCallback.BindUFunction(this, FName("SetScaleTimelineFinish"));
}

void ABlackhole::BeginPlay()
{
	Super::BeginPlay();

	//ScaleTimeline
	if (FloatCurve != nullptr) {
		MeshTimeline->SetTimelineLength(TimelineLength);
		MeshTimeline->AddInterpFloat(FloatCurve, floatTimelineCallback,FName("Alpha"));
		MeshTimeline->SetTimelineFinishedFunc(floatTimelineFinishedCallback);
		MeshTimeline->PlayFromStart();
		UE_LOG(LogTemp, Warning, TEXT("Start"));
	}

}

void ABlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlackhole::SetScaleTimelineUpdate(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"),Value);
}

void ABlackhole::SetScaleTimelineFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("fininsh"));
}

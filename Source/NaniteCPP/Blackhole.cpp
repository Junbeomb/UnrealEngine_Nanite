
#include "Blackhole.h"


ABlackhole::ABlackhole()
{
	PrimaryActorTick.bCanEverTick = true;

	//��Ʈ ������Ʈ ���� �� �Ž�������Ʈ ����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BlackholeBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackholeBaseMesh"));
	BlackholeBaseMesh->SetupAttachment(RootComponent);

	//��Ȧ �Ž� ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Blackhole/SM_BlackholeMesh"));
	BlackholeBaseMesh->SetStaticMesh(MeshAsset.Object);


	//��Ȧ ���� Timeline
	MeshTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("Timeline"));
		//� curve�� ����
	ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blackhole/FC_BlackholeSizeIncreaseCurve"));
	FloatCurve = Curve.Object;
	TimelineLength = 3.f;
		//callback�Լ�bind
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

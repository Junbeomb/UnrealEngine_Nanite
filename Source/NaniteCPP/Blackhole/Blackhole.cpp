
#include "Blackhole.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Components/SphereComponent.h"

ABlackhole::ABlackhole()
{
	PrimaryActorTick.bCanEverTick = false;

	DieToggle = false;


	//��Ʈ ������Ʈ ���� �� �Ž�������Ʈ ����
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

	//����Ȧ �Ž� ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Blackhole/SM_BlackholeMesh"));
	BlackholeBaseMesh->SetStaticMesh(MeshAsset.Object);

	//����Ȧ ���� Timeline
		MeshTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("MeshTimeline"));
		//� curve�� ����
		ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blackhole/Curve/FC_BlackholeSizeIncreaseCurve"));
		MeshCurve = Curve.Object;
		//callback�Լ�bind
		floatTimelineCallback.BindUFunction(this,FName("SetScaleTimelineUpdate"));
		floatTimelineFinishedCallback.BindUFunction(this, FName("SetScaleTimelineFinish"));

	//Range Timeline
		RangeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RangeTimeline"));
		//callback�Լ�bind
		RangeTimelineFinishedCallback.BindUFunction(this, FName("RangeTimelineFinish"));
		//� curve�� ����
		ConstructorHelpers::FObjectFinder<UCurveFloat> PullCurve(TEXT("/Game/Blackhole/Curve/FC_PullRange"));
		PullRangeCurve = PullCurve.Object;
		ConstructorHelpers::FObjectFinder<UCurveFloat> DFCurve(TEXT("/Game/Blackhole/Curve/FC_DFRange"));
		DFRangeCurve = DFCurve.Object;
		//callback�Լ�bind
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

	//RangeTimeline Ŀ�갡 ��� null�� �ƴҶ�
	if (PullRangeCurve && DFRangeCurve) {
		RangeTimeline->AddInterpFloat(PullRangeCurve, PullRangeTimelineCallback, FName("PullRange"));
		RangeTimeline->AddInterpFloat(DFRangeCurve, DFRangeTimelineCallback, FName("DFRange"));

		RangeTimeline->SetTimelineFinishedFunc(RangeTimelineFinishedCallback);

		RangeTimeline->PlayFromStart();
	}

}

void ABlackhole::SetScaleTimelineUpdate(float Value)
{
	//����Ȧ��Mesh ũ�Ⱑ Timeline�� ���� �þ�ų� �پ��
	BlackholeBaseMesh->SetWorldScale3D({ Value,Value,Value });
}

void ABlackhole::SetScaleTimelineFinish()
{
	//�پ��� Timeline�� ������
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

	//Delay -> 6�ʵ� Mesh�پ��� Animation����
	TimerDelegate.BindLambda([&]
	{
			DieToggle = true;
			MeshTimeline->SetPlayRate(10.f);
			MeshTimeline->Reverse();
	});

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, 6, false);

}


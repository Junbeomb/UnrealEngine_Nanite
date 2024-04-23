
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

	//��Ʈ ������Ʈ ���� �� �Ž�������Ʈ ����
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
	PullRange->SetHiddenInGame(false);

	//��Ȧ �Ž� ����
	//ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/1_Blackhole/SM_BlackholeMesh"));
	//BlackholeBaseMesh->SetStaticMesh(BlackholeStatic);

	//��Ȧ ���� Timeline
		MeshTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MeshTimeline"));
		//callback�Լ�bind
		floatTimelineCallback.BindUFunction(this,FName("SetScaleTimelineUpdate"));
		floatTimelineFinishedCallback.BindUFunction(this, FName("SetScaleTimelineFinish"));

	//Range Timeline (�� ���� Timeline�ȿ� �������� curve ����)
		RangeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RangeTimeline"));
		RangeTimelineFinishedCallback.BindUFunction(this, FName("RangeTimelineFinish"));

		//callback�Լ�bind
		PullRangeTimelineCallback.BindUFunction(this, FName("PullRangeTimelineUpdate"));
		DFRangeTimelineCallback.BindUFunction(this, FName("DFRangeTimelineUpdate"));
		FoliageRangeTimelineCallback.BindUFunction(this, FName("FoliageRangeTimelineUpdate"));

		//DFRange�� ��ġ��
		TurnOffDFRange -> OnComponentBeginOverlap.AddDynamic(this, &ABlackhole::OverlapDFRange);
		//pullRange�� ��ġ��
		PullRange -> OnComponentBeginOverlap.AddDynamic(this, &ABlackhole::OverlapPullRange);

	//DFStartRadius Ÿ�Ӷ���
		DFStartRadiusTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DFSTartRadiusTimeline"));
		DFStartRadiusTimelineCallback.BindUFunction(this, FName("DFStartRadiusTimelineUpdate"));
		DFStartRadiusTimelineFinishedCallback.BindUFunction(this, FName("DFStartRadiusTimelineFinish"));
}

void ABlackhole::BeginPlay()
{
	Super::BeginPlay();

	//ScaleTimeline
	if (MeshCurve) {
		MeshTimeline->AddInterpFloat(MeshCurve, floatTimelineCallback,FName("MeshScale"));
		MeshTimeline->SetTimelineFinishedFunc(floatTimelineFinishedCallback);
		MeshTimeline->PlayFromStart();
	}

	//Die
	DieBlackhole();

	//BP_BH �� �ٲ�� �Ŷ�� �˷��ֱ�
	if (FoliageToBPActor->GetChildActor()) {
		AFoliageInfluencer* FoliageInfluencer = Cast<AFoliageInfluencer>(FoliageToBPActor->GetChildActor());
		if(FoliageInfluencer)
			FoliageInfluencer->IsBlackholeInfluencer = true;
	}

	//RangeTimeline Ŀ�갡 ��� null�� �ƴҶ�
	if (PullRangeCurve && DFRangeCurve && FoliageRangeCurve) {
		RangeTimeline->AddInterpFloat(PullRangeCurve, PullRangeTimelineCallback, FName("PullRange"));
		RangeTimeline->AddInterpFloat(DFRangeCurve, DFRangeTimelineCallback, FName("DFRange"));
		RangeTimeline->AddInterpFloat(FoliageRangeCurve, FoliageRangeTimelineCallback, FName("FoliageRange"));

		RangeTimeline->SetTimelineFinishedFunc(RangeTimelineFinishedCallback);

		RangeTimeline->PlayFromStart();
	}

	//DistanceField ���� �����ϴ� Ÿ�Ӷ���
	if (DFStartRadiusCurve) {
		DFStartRadiusTimeline->AddInterpFloat(DFStartRadiusCurve, DFStartRadiusTimelineCallback, FName("DFStartRadius"));
		DFStartRadiusTimeline->SetTimelineFinishedFunc(DFStartRadiusTimelineFinishedCallback);
		DFStartRadiusTimeline->PlayFromStart();
	}

}


//��Ȧ �޽� ������ Ÿ�Ӷ���
void ABlackhole::SetScaleTimelineUpdate(float Value)
{
	BlackholeBaseMesh->SetWorldScale3D({ Value,Value,Value });
}

void ABlackhole::SetScaleTimelineFinish()
{
	if (DieToggle) {
		Destroy();
		return;
	}
}

//Range Ÿ�Ӷ��ε�
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
	//UE_LOG(LogTemp, Warning, TEXT("Range Timeline Finish"));
}

//DFStartRadius Ÿ�Ӷ���
void ABlackhole::DFStartRadiusTimelineUpdate(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Value);
	DFStartRadius = Value;
}

void ABlackhole::DFStartRadiusTimelineFinish()
{
	//UE_LOG(LogTemp, Warning, TEXT("DFStartRaiuds Timeline Finished"));
}


//PullRange�� actor �� ��ġ��
void ABlackhole::OverlapPullRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UBlackholeCompBase* BHComp = Cast<UBlackholeCompBase>(OtherActor->GetComponentByClass(UBlackholeCompBase::StaticClass()));
	if (BHComp) {
		if (!BHComp->GetIsPull()) { //�ش� ��ü�� ���� ���� ��������
			BHComp->SetPullOn(this, GetActorLocation());
		}
	}

}

void ABlackhole::OverlapDFRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UBlackholeCompBase* BHComp = Cast<UBlackholeCompBase>(OtherActor->GetComponentByClass(UBlackholeCompBase::StaticClass()));
	if (BHComp) {
		BHComp->SetDFOff(false);
	}
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

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, 12, false);

}



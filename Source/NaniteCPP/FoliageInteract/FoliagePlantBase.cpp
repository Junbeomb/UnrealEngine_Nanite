
#include "FoliagePlantBase.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedFoliageActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Containers/UnrealString.h"

AFoliagePlantBase::AFoliagePlantBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	OverlappingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlappingSphere"));
	OverlappingSphere->SetupAttachment(MeshComponent);
	OverlappingSphere->SetSphereRadius(32.f);

	OverlappingCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlappingCapsule"));
	OverlappingCapsule->SetupAttachment(MeshComponent);
	OverlappingCapsule->SetCapsuleHalfHeight(44.f);
	OverlappingCapsule->SetCapsuleRadius(22.f);

	OverlappingBlockPlayer = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlappingBlockPlayer"));
	OverlappingBlockPlayer->SetupAttachment(RootComponent);
	OverlappingBlockPlayer->SetCapsuleHalfHeight(44.f);
	OverlappingBlockPlayer->SetCapsuleRadius(22.f);

	SoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEffect"));
	//SoundEffect->SetSound();

	//pullRange�� ��ġ��
	OverlappingCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFoliagePlantBase::OverlapSphereOrCapsule);

	overlapIsSphere = false;
	soundOn = false;
	sizeBlockPlayer = 200.f;
	sizeNoPhysics = 350.f;
	isVeryLargePhysics = false;
	//Influencers�� �ϳ��� ���;� �������Ʈ�� ��������Ƿ� �⺻���� true
	IsInfluencersInRange = true;
	//������ ��ȯ����.
	IsSlowFoliageReturn = false;
	TimelineFinishDoOnce = false;

	//BlendWeight �ѹ��� �����Ű�� �Լ�
	DoOnce = false;

	//Fast Timeline ����
	FastTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("FastTimeline"));
	//Slow Timeline ����
	SlowTimeline = CreateDefaultSubobject< UTimelineComponent>(TEXT("SlowTimeline"));
	//callback�Լ�bind
	floatTimelineCallback.BindUFunction(this, FName("BlendWeightTimelineUpdate"));
	floatTimelineFinishedCallback.BindUFunction(this, FName("BlendWeightTimelineFinish"));

	
}

// Called when the game starts or when spawned
void AFoliagePlantBase::BeginPlay()
{
	Super::BeginPlay();

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

	float physicsRadius = MeshComponent->Bounds.SphereRadius;
	if (physicsRadius < sizeBlockPlayer) {
		//���� ������ �Ĺ��� ������ �÷��̾ ���� ����.
		OverlappingBlockPlayer->DestroyComponent();
	}

	//�ſ�ū �Ĺ�����
	isVeryLargePhysics = physicsRadius > sizeNoPhysics;

	//���� ���� ��� FoliageInfluencer�� array �� ���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFoliageInfluencer::StaticClass(), AllFoliageInfluencers);
	FoliageInfluencersLen = AllFoliageInfluencers.Num();

	//���� ���� Foliage�� �����ͼ� ù��°���� WorldFoliage�� ���
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), Temp);
	if(Temp.Num() > 0)
		WorldFoliage = Cast<AInstancedFoliageActor>(Temp[0]);


	//���� FoliageInfluencer �ִ��� Ȯ���ϱ�
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliagePlantBase::checkToFoliageInfluencer,0.05, false);

}

void AFoliagePlantBase::checkToFoliageInfluencer() {

	//UE_LOG(LogTemp, Warning, TEXT("%d"), FoliageInfluencersLen);
	//�ڽ��� �Ÿ� �ۿ� �ִ� FoliageInfluencer ���ֱ�
	for (AActor* a : AllFoliageInfluencers) {
		AFoliageInfluencer* FI = Cast<AFoliageInfluencer>(a);
		float distance = GetDistanceTo(FI);
		float radius = FI->PhysicsRadius + 100;

		if (distance > radius) {
			--FoliageInfluencersLen;
		}
	}

	//���࿡ �ϳ��� �Ÿ� �ȿ� �ִ�
	if (FoliageInfluencersLen > 0) {
		FoliageInfluencersLen = AllFoliageInfluencers.Num();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliagePlantBase::checkToFoliageInfluencer, 0.1, false); //Influencers �� ��� �ֺ��� ���������� �ݺ�
		IsInfluencersInRange = true;
		
	}
	else { //�ϳ��� �Ÿ��ȿ� Influencers �� ����.
		IsInfluencersInRange = false;	
	}

	if (!IsInfluencersInRange) { // ������ Influencers�� ������
		if (!DoOnce) { //�ѹ��� ����
			DoOnce = true;
			NoInfluencersInRangeFunc();
		}
	}
	if (IsInfluencersInRange) { //������ Influencers�� �ϳ��� ������.
		FastTimeline->Stop();
		SlowTimeline->Stop();

		MeshComponent->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("b_Base")), 1.0f, false, true);
		DoOnce = false;
	}

	//��ݺ��� ������ �ʴ���� foliage-Blueprint�� �ݺ��ǹǷ�(���� �ֺ��� ������ BP�� ���ϹǷ�) �ٽ� ����.
}

//������ InfluencersInRange�� �ƹ��͵� ������ �����ϴ� �Լ�.
void AFoliagePlantBase::NoInfluencersInRangeFunc()
{
	if (isVeryLargePhysics) { //�ſ�ū �Ĺ��̸� �ٷ� foliage�� ��ȯ
		AFoliagePlantBase::BlendWeightTimelineFinish();
	}
	else { //���� �Ĺ��̸� Blend Weight ����
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
		int classIndex;

		for (UActorComponent* Comp : Temp) {

			UInstancedStaticMeshComponent* InstancedMesh = Cast<UInstancedStaticMeshComponent>(Comp);

			FString LeftS;
			FString RightS;

			MeshComponent->GetSkeletalMeshAsset()->GetName().Split(TEXT("SK_"), &LeftS, &RightS);
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


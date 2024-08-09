// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageRockBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Blending/Comp_BlendMesh.h"
#include "InstancedFoliageActor.h"
#include "FoliageInfluencer.h"

// Sets default values
AFoliageRockBase::AFoliageRockBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

	//BlendWeight �ѹ��� �����Ű�� �Լ�
	DoOnce = false;
	//Influencers�� �ϳ��� ���;� �������Ʈ�� ��������Ƿ� �⺻���� true
	IsInfluencersInRange = true;

}

// Called when the game starts or when spawned
void AFoliageRockBase::BeginPlay()
{
	Super::BeginPlay();
	Comp_Blend->D_FinishBlending.BindUObject(this, &AFoliageRockBase::ReturnToFoliage);
	Comp_Blend->D_JustGo.BindUObject(this, &AFoliageRockBase::ReturnToFoliage);
}

void AFoliageRockBase::ReturnToFoliage()
{
	//���� ���� ��� FoliageInfluencer�� array �� ���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFoliageInfluencer::StaticClass(), AllFoliageInfluencers);
	FoliageInfluencersLen = AllFoliageInfluencers.Num();

	//���� ���� Foliage�� �����ͼ� ù��°���� WorldFoliage�� ���
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), Temp);
	if (Temp.Num() > 0)
		WorldFoliage = Cast<AInstancedFoliageActor>(Temp[0]);

	//���� FoliageInfluencer �ִ��� Ȯ���ϱ�
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageRockBase::checkToFoliageInfluencer, 0.05, false);
}

void AFoliageRockBase::checkToFoliageInfluencer()
{
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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageRockBase::checkToFoliageInfluencer, 0.1, false); //Influencers �� ��� �ֺ��� ���������� �ݺ�
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
		DoOnce = false;
	}

	//��ݺ��� ������ �ʴ���� foliage-Blueprint�� �ݺ��ǹǷ�(���� �ֺ��� ������ BP�� ���ϹǷ�) �ٽ� ����.
}

void AFoliageRockBase::NoInfluencersInRangeFunc()
{
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);

	if (!FinishDoOnce) {
		FinishDoOnce = true;

		//WorldFoliage�� �ִ� �Ĺ��� ��� �����ͼ� ����.
		//Beginplay���� WorldFoliage�� 0��°�� ���������Ƿ� landscape�� ù��° Foliage���� �����´�.
		//���� OpenWorldó�� Landscape�������� ���ĳ��� �ʿ��� �����ö����� InstancedFoliageActor[0] ��°�� ��� ������ �޽��� �־���Ѵ�.
		TArray<UActorComponent*> Temp;
		WorldFoliage->GetComponents(UInstancedStaticMeshComponent::StaticClass(), Temp);


		int count = 0;
		int classIndex = 0;

		for (UActorComponent* Comp : Temp) {

			UInstancedStaticMeshComponent* InstancedMesh = Cast<UInstancedStaticMeshComponent>(Comp);

			FString LeftS;
			FString RightS;

			if (Comp_Blend->IsLow()) {
				MeshComponent->GetStaticMesh()->GetName().Split(TEXT("SM_"), &LeftS, &RightS);
				RightS = "L_" + RightS;
			}
			else {
				MeshComponent->GetStaticMesh()->GetName().Split(TEXT("SM_"), &LeftS, &RightS);
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Blending/Comp_BlendMesh.h"
#include "InstancedFoliageActor.h"
#include "FoliageInfluencer.h"

// Sets default values
AFoliageBase::AFoliageBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));


	DoOnce = false;
	//Influencers�� �ϳ��� ���;� �������Ʈ�� ��������Ƿ� �⺻���� true
	IsInfluencersInRange = true;
}

// Called when the game starts or when spawned
void AFoliageBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("FoliageBase"));
	Comp_Blend->D_FinishBlending.BindUObject(this, &AFoliageBase::ReturnToFoliage);
	Comp_Blend->D_JustGo.BindUObject(this, &AFoliageBase::ReturnToFoliage);
}

void AFoliageBase::ReturnToFoliage()
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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageBase::checkToFoliageInfluencer, 0.05, false);

	DestroyCompFunc();
}

void AFoliageBase::checkToFoliageInfluencer()
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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageBase::checkToFoliageInfluencer, 0.1, false); //Influencers �� ��� �ֺ��� ���������� �ݺ�
		IsInfluencersInRange = true;

	}
	else { //�ϳ��� �Ÿ��ȿ� Influencers �� ����.
		IsInfluencersInRange = false;
	}

	if (!IsInfluencersInRange) { // ������ Influencers�� ������
		if (!DoOnce) { //�ѹ��� ����
			DoOnce = true;
			GoCustomFunc();
		}
	}
	if (IsInfluencersInRange) { //������ Influencers�� �ϳ��� ������.
		TurnOffAnim();
		DoOnce = false;
	}

	//��ݺ��� ������ �ʴ���� foliage-Blueprint�� �ݺ��ǹǷ�(���� �ֺ��� ������ BP�� ���ϹǷ�) �ٽ� ����.
}

void AFoliageBase::NoInfluencersInRangeFunc()
{
	SetCollisionSimulate();

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

			GetMeshName(LeftS, RightS);
			if (Comp_Blend->IsLow()) {
				RightS = "L_" + RightS;
			}
			else {
				RightS = "H_" + RightS;
			}

			FString value = *InstancedMesh->GetStaticMesh()->GetName();

			bool isContain = value.Contains(*RightS, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			if (isContain) {
				classIndex = count;
			}
			++count;
		}

		AddFoliageInstance(Temp[classIndex]);
		
	
		Destroy();
	}
}

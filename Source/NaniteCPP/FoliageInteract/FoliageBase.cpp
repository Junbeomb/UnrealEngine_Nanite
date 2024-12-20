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
	//Influencers가 하나라도 들어와야 블루프린트가 만들어지므로 기본값은 true
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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageBase::checkToFoliageInfluencer, 0.05, false);

	DestroyCompFunc();
}

void AFoliageBase::checkToFoliageInfluencer()
{
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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageBase::checkToFoliageInfluencer, 0.1, false); //Influencers 가 모두 주변에 없을때까지 반복
		IsInfluencersInRange = true;

	}
	else { //하나도 거리안에 Influencers 가 없다.
		IsInfluencersInRange = false;
	}

	if (!IsInfluencersInRange) { // 주위에 Influencers가 없으면
		if (!DoOnce) { //한번만 실행
			DoOnce = true;
			GoCustomFunc();
		}
	}
	if (IsInfluencersInRange) { //주위에 Influencers가 하나라도 있으면.
		TurnOffAnim();
		DoOnce = false;
	}

	//재반복을 해주지 않더라고 foliage-Blueprint가 반복되므로(내가 주변에 있으면 BP로 변하므로) 다시 실행.
}

void AFoliageBase::NoInfluencersInRangeFunc()
{
	SetCollisionSimulate();

	if (!FinishDoOnce) {
		FinishDoOnce = true;

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

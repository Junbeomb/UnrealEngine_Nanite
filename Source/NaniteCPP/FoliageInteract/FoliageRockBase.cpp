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

	//BlendWeight 한번만 실행시키는 함수
	DoOnce = false;
	//Influencers가 하나라도 들어와야 블루프린트가 만들어지므로 기본값은 true
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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageRockBase::checkToFoliageInfluencer, 0.05, false);
}

void AFoliageRockBase::checkToFoliageInfluencer()
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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFoliageRockBase::checkToFoliageInfluencer, 0.1, false); //Influencers 가 모두 주변에 없을때까지 반복
		IsInfluencersInRange = true;

	}
	else { //하나도 거리안에 Influencers 가 없다.
		IsInfluencersInRange = false;
	}

	if (!IsInfluencersInRange) { // 주위에 Influencers가 없으면
		if (!DoOnce) { //한번만 실행
			DoOnce = true;
			NoInfluencersInRangeFunc();
		}
	}
	if (IsInfluencersInRange) { //주위에 Influencers가 하나라도 있으면.
		DoOnce = false;
	}

	//재반복을 해주지 않더라고 foliage-Blueprint가 반복되므로(내가 주변에 있으면 BP로 변하므로) 다시 실행.
}

void AFoliageRockBase::NoInfluencersInRangeFunc()
{
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);

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

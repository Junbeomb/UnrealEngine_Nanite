

#include "WindBomb.h"
#include "Kismet/GameplayStatics.h"
#include "WindManager.h"
#include "TimerManager.h"
#include "WindStructData.h"
#include "../Blackhole/Blackhole.h"
#include "../InteractionSystem/InteractStatue.h"



AWindBomb::AWindBomb()
{

	PrimaryActorTick.bCanEverTick = false;

	//Timeline
	WindTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("WindTL"));
	RadiusTLCallback.BindUFunction(this, FName("SetRadiusTLUpdate"));
	StrengthTLCallback.BindUFunction(this, FName("SetStrengthTLUpdate"));
	WindTLFinishedCallback.BindUFunction(this, FName("SetScaleTimelineFinish"));

}


void AWindBomb::BlackholeFunc(ABlackhole& bh)
{
	//바람 제거
	bh.D_SoonDie.AddLambda([this]() {
		Destroy();
	});

	//windmanager 가져오기
	windManager = Cast<AWindManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWindManager::StaticClass()));
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AWindBomb::StartWind, 0.2f, true);
}

void AWindBomb::StatueFunc(AInteractStatue& is)
{
	//바람 제거
	is.D_Die.AddLambda([this]() {
		Destroy();
	});
	//windmanager 가져오기
	windManager = Cast<AWindManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWindManager::StaticClass()));

	//Timeline
	if (StatueRadiusCurve && StatueStrengthCurve) {
		WindTimeline->AddInterpFloat(StatueRadiusCurve, RadiusTLCallback, FName("Radius"));
		WindTimeline->AddInterpFloat(StatueStrengthCurve, StrengthTLCallback, FName("Strength"));

		WindTimeline->SetTimelineFinishedFunc(WindTLFinishedCallback);

		WindTimeline->PlayFromStart();
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWindBomb::StartWind, 0.2f, true);
}

void AWindBomb::StartWind()
{
	//UE_LOG(LogTemp, Warning, TEXT("StartWind"));

	FWINDDATA tempData;
	tempData.Duration = Duration;
	tempData.StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if(StrengthCurve)
		tempData.StrengthCurve = StrengthCurve;
	tempData.S_WindStartLocRad = FVector4(GetActorLocation(), Radius);
	tempData.S_WindStartVelStr = FVector4(0, 0, 1, Strength);

	windManager->AddWindAtWindData(tempData);

}


//Timeline
void AWindBomb::SetScaleTimelineFinish()
{
}

void AWindBomb::SetRadiusTLUpdate(float Value)
{
	Radius = Value;
}

void AWindBomb::SetStrengthTLUpdate(float Value)
{
	Strength = Value;
}




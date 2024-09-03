

#include "WindBomb.h"
#include "Kismet/GameplayStatics.h"
#include "WindManager.h"
#include "TimerManager.h"
#include "WindStructData.h"
#include "../Blackhole/Blackhole.h"


AWindBomb::AWindBomb()
{
 
	PrimaryActorTick.bCanEverTick = false;

}


void AWindBomb::BlackholeFunc(ABlackhole& bh)
{
	//�ٶ� ����
	bh.D_SoonDie.AddLambda([this]() {
		Destroy();
	});

	//windmanager ��������
	windManager = Cast<AWindManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWindManager::StaticClass()));
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AWindBomb::StartWind, 0.5f, true);
}

void AWindBomb::StatueFunc(AInteractStatue& is)
{
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




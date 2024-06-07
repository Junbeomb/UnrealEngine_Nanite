// Fill out your copyright notice in the Description page of Project Settings.


#include "WindBomb.h"
#include "Kismet/GameplayStatics.h"
#include "WindManager.h"
#include "TimerManager.h"
#include "WindStructData.h"



// Sets default values
AWindBomb::AWindBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}



// Called when the game starts or when spawned
void AWindBomb::BeginPlay()
{
	Super::BeginPlay();

	BlackholeFunc();
	
}

void AWindBomb::BlackholeFunc()
{
	//windmanager 가져오기
	windManager = Cast<AWindManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWindManager::StaticClass()));
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AWindBomb::StartWind, 0.5f, true);
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




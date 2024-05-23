// Fill out your copyright notice in the Description page of Project Settings.


#include "WindManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"




// Sets default values
AWindManager::AWindManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindNiagara"));

	InputGridResolution = 256;
	SimulationSizeWS = 4096.f;
}

void AWindManager::BeginPlay()
{
	Super::BeginPlay();

	ResetVariable();

}

void AWindManager::ResetVariable()
{
	if (!Niagara) return;

	SetInitialVariable();
	SetPlayerPawnLocation();

	SetActorTickEnabled(true);
}

void AWindManager::SetInitialVariable()
{
	Niagara->SetNiagaraVariableFloat("TargetFPS", 60.f);

	Niagara->SetNiagaraVariableFloat("SimulationSizeWS", SimulationSizeWS);
	Niagara->SetNiagaraVariableInt("WindGridResolution", InputGridResolution);
	if (ParamCollection) {
		PCI = GetWorld()->GetParameterCollectionInstance(ParamCollection);
		PCI->SetScalarParameterValue(FName("SimulationSizeWS"), SimulationSizeWS);
		PCI->SetScalarParameterValue(FName("Wind Resolution"), InputGridResolution);
	}
	Niagara->SetNiagaraVariableInt("InputGridResolution", InputGridResolution);
	WindRenderTarget->ResizeTarget(InputGridResolution, InputGridResolution);
}

void AWindManager::SetPlayerPawnLocation()
{
	APawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	PlayerComponent = playerPawn->GetRootComponent();
}


// Called every frame
void AWindManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerLocation = PlayerComponent->GetComponentLocation();
	WindStartLocationRadius.Empty();
	WindStartVelocityStrength.Empty();
	WindStructDataToTranslate();
}

void AWindManager::WindStructDataToTranslate()
{
	for (int i = 0; i < SWindData.Num() - 1; i++) {
		float TempDuration = SWindData[i].Duration;
		float TempStartTime = SWindData[i].StartTime;
		UCurveFloat* TempCF = SWindData[i].StrengthCurve;
		FVector4 TempSLR = SWindData[i].S_WindStartLocRad;
		FVector4 TempSVS = SWindData[i].S_WindStartVelStr;

		if (TempDuration <= 0) { //Duration�� ó������ 0�� �ٶ��� ����
			DeleteIndexList.Add(i);
			WindStartLocationRadius.Add(TempSLR);
			WindStartVelocityStrength.Add(TempSVS);
		}
		else {
			float TempTimeRate = UGameplayStatics::GetRealTimeSeconds(GetWorld()) - TempStartTime/TempDuration;

			if (TempTimeRate >= 1.0f) { //��� �ð� �Ѿ�� �ٶ� ����
				DeleteIndexList.Add(i);
			}

			FVector4 TempVector4;
			if (TempCF) {
				TempVector4 = FVector4(1.f, 1.f, 1.f, FMath::Max(TempCF->GetFloatValue(TempTimeRate), 0.0f));
			}
			WindStartLocationRadius.Add(TempSLR);
			WindStartVelocityStrength.Add(TempSVS * TempVector4);
		}
	}

	for (const int& a : DeleteIndexList) {
		SWindData.RemoveAt(a);
	}

	UE_LOG(LogTemp, Warning, TEXT("WindManager"));
}

void AWindManager::ResetTick()
{
	SetActorTickEnabled(false);
}


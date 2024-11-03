// Fill out your copyright notice in the Description page of Project Settings.


#include "WindManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector2D.h"

// Sets default values
AWindManager::AWindManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindNiagara"));

	InputGridResolution = 256.f;
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

	Niagara->SetNiagaraVariableFloat("SimulationSizeWS", SimulationSizeWS);
	Niagara->SetNiagaraVariableFloat("WindGridResolution", InputGridResolution);
	if (ParamCollection) {
		PCI = GetWorld()->GetParameterCollectionInstance(ParamCollection);
		PCI->SetScalarParameterValue(FName("SimulationSizeWS"), SimulationSizeWS);
		PCI->SetScalarParameterValue(FName("Wind Resolution"), InputGridResolution);
	}
	Niagara->SetNiagaraVariableFloat("InputGridResolution", InputGridResolution);
	WindRenderTarget->ResizeTarget(InputGridResolution, InputGridResolution);
}

void AWindManager::SetPlayerPawnLocation()
{
	APawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	PlayerComponent = playerPawn->GetRootComponent();
}


void AWindManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerLocation = PlayerComponent->GetComponentLocation();
	WindStartLocationRadius.Empty();
	WindStartVelocityStrength.Empty();
	WindStructDataToTranslate();
	CacluatePlayerPosition();
	CacluateWindPosition();
	SetGridVariable();
}

void AWindManager::WindStructDataToTranslate()
{
	TArray<int> DeleteIndexList;
	for (int i = 0; i < SWindData.Num(); i++) {
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
			float TempTimeRate = (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - TempStartTime)/TempDuration;

			if (TempTimeRate >= 1.0f) { //��� �ð� �Ѿ�� �ٶ� ����
				DeleteIndexList.Add(i);
			}
			//UE_LOG(LogTemp, Warning, TEXT("%f"), TempTimeRate);
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

	
}

void AWindManager::CacluatePlayerPosition()
{
	//player �������� niagara�� �ѱ��.
	FVector2D vector2d = { PlayerLocation.X,PlayerLocation.Y };
	vector2d = vector2d / (SimulationSizeWS / InputGridResolution);
	vector2d = FVector2D(FMath::RoundToInt32(vector2d.X),FMath::RoundToInt32(vector2d.Y));
	vector2d = vector2d * (SimulationSizeWS / InputGridResolution);
	Niagara->SetNiagaraVariableVec2("FollowGridLocation", vector2d);
}

void AWindManager::CacluateWindPosition()
{
	//player �� �̵��� �Ÿ���ŭ windpixel ���� �ݴ�� �̵�.(Render Target Texture������ �÷��̾ ������� �ʵ���)
	FColor tempColor = FColor(windLocation.X,windLocation.Y,0,0);
	PCI->SetVectorParameterValue(FName("WindRTLocation"), tempColor);

	FVector2D vector2d = { PlayerLocation.X,PlayerLocation.Y };
	vector2d = vector2d / (SimulationSizeWS / InputGridResolution);
	vector2d = FVector2D(FMath::RoundToInt32(vector2d.X), FMath::RoundToInt32(vector2d.Y));
	vector2d = vector2d * (SimulationSizeWS / InputGridResolution);
	windOffset = vector2d - windLocation;
	windLocation = vector2d;

	Niagara->SetNiagaraVariableVec2("WindGridPixelOffset", windOffset * ( - 1, -1 ) *InputGridResolution / SimulationSizeWS);
}

void AWindManager::SetGridVariable()
{
	Niagara->SetNiagaraVariableVec2("InputGridLocation", { PlayerLocation.X,PlayerLocation.Y });
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(Niagara, FName("WindCapsuleStartLocationAndRadius"), WindStartLocationRadius);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(Niagara, FName("WindCapsuleStartVelocityAndStrength"), WindStartVelocityStrength);

}

void AWindManager::ResetTick()
{
	SetActorTickEnabled(false);
}

//�ٶ����
void AWindManager::AddWindAtWindData(FWINDDATA wsd)
{
	float distance = FVector2D::DistSquared({ PlayerLocation.X, PlayerLocation.Y}, {wsd.S_WindStartLocRad.X, wsd.S_WindStartLocRad.Y});
	if(distance < pow(SimulationSizeWS * 0.7,2))
		SWindData.Add(wsd);
}


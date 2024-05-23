// Fill out your copyright notice in the Description page of Project Settings.


#include "WindManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"





// Sets default values
AWindManager::AWindManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindNiagara"));

	InputGridResolution = 256;
	SimulationSizeWS = 4096.f;
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
}

void AWindManager::PullDataToTranslate()
{
}

void AWindManager::ResetTick()
{
	SetActorTickEnabled(false);
}


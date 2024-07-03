#include "FinalChargeCenter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "../Blending/Comp_BlendMesh.h"


AFinalChargeCenter::AFinalChargeCenter()
{
	PrimaryActorTick.bCanEverTick = true;

	CenterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterMesh"));
	RootComponent = CenterMesh;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

	//TL_Dissapear
	TL_Dissapear = CreateDefaultSubobject<UTimelineComponent>(TEXT("TL_Dissapear"));
	EmissiveCallback.BindUFunction(this, FName("EmissiveUpdate"));
	WPOCallback.BindUFunction(this, FName("WPOUpdate"));
	TLDissapearFinishCallback.BindUFunction(this, FName("TLDissapearFinish"));
}


void AFinalChargeCenter::BeginPlay()
{
	Super::BeginPlay();
	
	DMI = CenterMesh->CreateDynamicMaterialInstance(0, CenterMesh->GetMaterial(0));

	Comp_Blend->D_StartBlending.BindUObject(this, &AFinalChargeCenter::BlendStart);
	Comp_Blend->D_FinishBlending.BindUObject(this, &AFinalChargeCenter::BlendFinish);
}

void AFinalChargeCenter::BlendStart()
{
	UE_LOG(LogTemp, Warning, TEXT("FinalChargeCenter : Blend Start"));

	//ScaleTimeline
	if (EmissiveCurve && WPOCurve) {
		TL_Dissapear->AddInterpFloat(EmissiveCurve, EmissiveCallback, FName("Emissive"));
		TL_Dissapear->AddInterpFloat(WPOCurve, WPOCallback, FName("WPO"));

		TL_Dissapear->SetTimelineFinishedFunc(TLDissapearFinishCallback);
		TL_Dissapear->PlayFromStart();
	}
}

void AFinalChargeCenter::BlendFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("FinalChargeCenter : Blend Finish"));

}
void AFinalChargeCenter::EmissiveUpdate(float Value)
{
	DMI->SetScalarParameterValue(FName("Emissive"),Value);
}

void AFinalChargeCenter::WPOUpdate(float Value)
{
	DMI->SetScalarParameterValue(FName("WPO"), Value);
}

void AFinalChargeCenter::TLDissapearFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("FinalChargeCenter : TL Finish"));
}

void AFinalChargeCenter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


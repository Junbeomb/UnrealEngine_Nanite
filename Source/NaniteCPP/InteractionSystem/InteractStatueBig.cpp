

#include "InteractStatueBig.h"

#include "Components/SphereComponent.h"
#include "InstancedFoliageActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Comp_InteractBase.h"
#include "Engine/PostProcessVolume.h"
#include "Math/UnrealMathUtility.h"



AInteractStatueBig::AInteractStatueBig()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	Bomb = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Bomb"));
	Bomb->SetupAttachment(RootComponent);

	WindNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindNS"));
	WindNS->SetupAttachment(RootComponent);

	CompBase = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_InteractBase"));

	Bomb->SetAutoActivate(false);
	WindNS->SetAutoActivate(false);

	BombDistance = 50000.f;

	//MassBlendTimeline
	MassBlendTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MassBlendTimeline"));
	MassBlendTimelineFinishedCallback.BindUFunction(this, FName("SetMassBlendTimelineFinish"));
	MassBlendTimelineUpdateCallback.BindUFunction(this, FName("SetMassBlendTimelineUpdate"));
	NormalAmplifyTimelineUpdateCallback.BindUFunction(this, FName("SetNormalAmplifyTimelineUpdate"));
	EmissiveTimelineUpdateCallback.BindUFunction(this, FName("SetEmissiveTimelineUpdate"));

	//ShakeSMTimeline
	ShakeSMTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ShakeSMTimeline"));
	ShakeSMTimelineFinishedCallback.BindUFunction(this, FName("SetShakeSMTimelineFinish"));
	ShakeSMTimelineUpdateCallback.BindUFunction(this, FName("SetShakeSMTImelineUpdate"));
	SizeSMTimelineUpdateCallback.BindUFunction(this, FName("SetSizeSMTImelineUpdate"));

	//PPVTimeline
	PPVTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PPVTimeline"));
	PPVTimelineFinishedCallback.BindUFunction(this, FName("SetPPVTimelineFinish"));
	PPVTimelineUpdateCallback.BindUFunction(this, FName("SetPPVTImelineUpdate"));


}

// Called when the game starts or when spawned
void AInteractStatueBig::BeginPlay()
{
	Super::BeginPlay();

	//월드내의 Foliage Static Mesh 리스트 가져오기
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), Temp);
	Temp[0]->GetComponents<UInstancedStaticMeshComponent>(InstancedMesh);
	
}
void AInteractStatueBig::PressEStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("PressEStart"));

	CompBase->DestroyThisComponentFunc();

	//Statue StaticMesh 움직이게하기
	if (StaticMesh) {
		for (int i = 0; i < StaticMesh->GetNumMaterials(); i++) {
			UMaterialInstanceDynamic* MID = StaticMesh->CreateDynamicMaterialInstance(i, StaticMesh->GetMaterial(i));
			ShakeSMDMIList.Add(MID);
		}
	}

	//postProcessVolume 조절
	PPV = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
	PPVIntensity = PPV->Settings.BloomIntensity;
	PPVCompensation = PPV->Settings.AutoExposureBias;

	UE_LOG(LogTemp, Warning, TEXT("%f"), PPV->Settings.BloomIntensity);

	if (ShakeSMCurve && SizeSMCurve) {
		ShakeSMTimeline->AddInterpFloat(ShakeSMCurve, ShakeSMTimelineUpdateCallback, FName("ShaekSM"));
		ShakeSMTimeline->AddInterpFloat(SizeSMCurve, SizeSMTimelineUpdateCallback, FName("SizeSM"));
		ShakeSMTimeline->SetTimelineFinishedFunc(ShakeSMTimelineFinishedCallback);
		ShakeSMTimeline->PlayFromStart();
	}

}

void AInteractStatueBig::StartMassBlend()
{
	//UE_LOG(LogTemp, Warning, TEXT("startMassBlend"));
	//D_BPStartBlend.Execute();

	if (ParamCollection) {
		PCI = GetWorld()->GetParameterCollectionInstance(ParamCollection);
		FVector BombCenter = GetActorLocation();
		PCI->SetVectorParameterValue(FName("MassCenter"), BombCenter);
		PCI->SetScalarParameterValue(FName("MassDivide"), 20.f);
		FLinearColor EColor;
		PCI->GetVectorParameterValue(FName("MassEmissiveColor"), EColor);

		PPV->Settings.bOverride_BloomIntensity = true;
		PPV->Settings.BloomIntensity = 8.0f;

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;



		//MassBlendTimeline
		if (MassBlendCurve && NormalAmplifyCurve && EmissiveCurve) {
			MassBlendTimeline->AddInterpFloat(MassBlendCurve, MassBlendTimelineUpdateCallback, FName("Blend"));
			MassBlendTimeline->AddInterpFloat(NormalAmplifyCurve, NormalAmplifyTimelineUpdateCallback, FName("NormalAmplify"));
			MassBlendTimeline->AddInterpFloat(EmissiveCurve, EmissiveTimelineUpdateCallback, FName("Emissive"));

			MassBlendTimeline->SetTimelineFinishedFunc(MassBlendTimelineFinishedCallback);

			MassBlendTimeline->PlayFromStart();
		}

		//4초뒤 PPV 값 원래대로 & distancefield 키기
		TimerDelegate.BindLambda([&]
		{
			if (PPVCurve) {
				PPVTimeline->AddInterpFloat(PPVCurve, PPVTimelineUpdateCallback, FName("Intensity"));
				PPVTimeline->SetTimelineFinishedFunc(PPVTimelineFinishedCallback);
				PPVTimeline->PlayFromStart();
			}

			//df 키기
			for (UInstancedStaticMeshComponent* a : InstancedMesh) {
				a->SetAffectDistanceFieldLighting(true);
			}

		});

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 4, false);
	}

}

//==================BlendMassTimeline====================
//==================BlendMassTimeline====================
//==================BlendMassTimeline====================

void AInteractStatueBig::SetMassBlendTimelineFinish()
{
	//D_BPFinishBlend.Execute();
	Bomb->DestroyComponent();
}

void AInteractStatueBig::SetMassBlendTimelineUpdate(float Value)
{
	BlendRadius = BombDistance * Value;

	PCI->SetScalarParameterValue(FName("MassRadius"), BlendRadius);
	Bomb->SetNiagaraVariableVec3("Scale Factor", { BlendRadius / 50, BlendRadius / 50, BlendRadius / 50 });
	WindNS->SetNiagaraVariableFloat("Radius", BlendRadius);
}

void AInteractStatueBig::SetNormalAmplifyTimelineUpdate(float Value)
{
	PCI->SetScalarParameterValue(FName("MassAmplify"), Value);
}

void AInteractStatueBig::SetEmissiveTimelineUpdate(float Value)
{
	PCI->SetVectorParameterValue(FName("MassEmissiveColor"), Value * InitialEmissiveColor);
}

//==================BlendMassTimeline====================
//==================BlendMassTimeline====================
//==================BlendMassTimeline====================

//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================
void AInteractStatueBig::SetShakeSMTimelineFinish()
{
	StaticMesh->DestroyComponent();

	//NiagaraComponent 세팅
	Bomb->Activate(true);
	WindNS->Activate(true);

	Bomb->SetNiagaraVariableFloat("Lifetime", BombDistance / 1000); //1초당 1000cm만큼 이동하므로
	WindNS->SetNiagaraVariableVec2("PersonLocation", { GetActorLocation().X,GetActorLocation().Y });
	WindNS->SetNiagaraVariableInt("Resolution", 256);
	WindNS->SetNiagaraVariableFloat("SimulationSize", 4096.f);

	StartMassBlend();
}
void AInteractStatueBig::SetShakeSMTimelineUpdate(float Value)
{
	AddActorLocalOffset({ sin(double(Value) * 10000) * 3 ,sin(double(Value) * 10000) * 3 ,Value });
	for (UMaterialInstanceDynamic* a : ShakeSMDMIList) {
		FLinearColor TempLinear = { 0.f, 0.f, (Value * 5000.f + 1.f), 1.0f };
		a->SetVectorParameterValue(FName("EmissiveColor"), TempLinear);
		a->SetScalarParameterValue(FName("PannerSpeed"), Value * 10 + 1.0f);
	}
}
void AInteractStatueBig::SetSizeSMTimelineUpdate(float Value)
{
	StaticMesh->SetWorldScale3D({ Value,Value,Value });
}

//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================

//==================PPVTimeline====================
//==================PPVTimeline====================
//==================PPVTimeline====================
void AInteractStatueBig::SetPPVTimelineFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("Finish PPV"));
}

void AInteractStatueBig::SetPPVTimelineUpdate(float Value)
{
	float IntensityValue = FMath::Max(8.f * Value,PPVIntensity);
	PPV->Settings.BloomIntensity = IntensityValue;
	UE_LOG(LogTemp, Warning, TEXT("%f"), IntensityValue);
}
//==================PPVTimeline====================
//==================PPVTimeline====================
//==================PPVTimeline====================

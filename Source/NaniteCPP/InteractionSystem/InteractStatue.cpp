// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractStatue.h"

#include "Components/SphereComponent.h"
#include "InstancedFoliageActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AInteractStatue::AInteractStatue()
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

	BombDistance = 4000.f;

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

}

// Called when the game starts or when spawned
void AInteractStatue::BeginPlay()
{
	Super::BeginPlay();
	//CompBase->SetDynamicMaterial(StaticMesh, false);

	//월드내의 Foliage Static Mesh 리스트 가져오기
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), Temp);
	Temp[0]->GetComponents<UInstancedStaticMeshComponent>(InstancedMesh);

	//Low 폴리만 DMI로 변경
	for (UInstancedStaticMeshComponent* a : InstancedMesh) {
		if (a->GetStaticMesh()->GetName().Contains("SM_L_")) {
			for (int i = 0; i < a->GetNumMaterials(); i++) {
				UMaterialInstanceDynamic* TempDMI = a->CreateDynamicMaterialInstance(i, a->GetMaterial(i));
				DMIList.Add(TempDMI);
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *StaticMesh->GetName());

}

void AInteractStatue::PressEStart()
{
	UE_LOG(LogTemp, Warning, TEXT("PressEStart"));
	CompBase->TurnOffHover();
	CompBase->DestroyComponent();

	if (ParamCollection) {
		PCI = GetWorld()->GetParameterCollectionInstance(ParamCollection);
		FVector BombCenter = GetActorLocation();
		PCI->SetVectorParameterValue(FName("MassCenter"), BombCenter);
	}

	//Statue StaticMesh 움직이게하기
	if (StaticMesh) {
		for (int i = 0; i < StaticMesh->GetNumMaterials(); i++) {
			UMaterialInstanceDynamic* MID = StaticMesh->CreateDynamicMaterialInstance(i, StaticMesh->GetMaterial(i));
			ShakeSMDMIList.Add(MID);
		}
	}

	if (ShakeSMCurve && SizeSMCurve) {
		ShakeSMTimeline->AddInterpFloat(ShakeSMCurve, ShakeSMTimelineUpdateCallback, FName("ShaekSM"));
		ShakeSMTimeline->AddInterpFloat(SizeSMCurve, SizeSMTimelineUpdateCallback, FName("SizeSM"));
		ShakeSMTimeline->SetTimelineFinishedFunc(ShakeSMTimelineFinishedCallback);
		ShakeSMTimeline->PlayFromStart();
	}

}

void AInteractStatue::StartMassBlend()
{
	//UE_LOG(LogTemp, Warning, TEXT("startMassBlend"));
	\
	//D_BPStartBlend.Execute();

	PCI->SetScalarParameterValue(FName("MassDivide"), 20.f);
	if (PCI) {
		FLinearColor EColor;
		PCI->GetVectorParameterValue(FName("MassEmissiveColor"), EColor);
	}

	//MassBlendTimeline
	if (MassBlendCurve && NormalAmplifyCurve && EmissiveCurve) {
		MassBlendTimeline->AddInterpFloat(MassBlendCurve, MassBlendTimelineUpdateCallback, FName("Blend"));
		MassBlendTimeline->AddInterpFloat(NormalAmplifyCurve, NormalAmplifyTimelineUpdateCallback, FName("NormalAmplify"));
		MassBlendTimeline->AddInterpFloat(EmissiveCurve, EmissiveTimelineUpdateCallback, FName("Emissive"));

		MassBlendTimeline->SetTimelineFinishedFunc(MassBlendTimelineFinishedCallback);

		MassBlendTimeline->PlayFromStart();
	}
}

//==================BlendMassTimeline====================
//==================BlendMassTimeline====================
//==================BlendMassTimeline====================

void AInteractStatue::SetMassBlendTimelineFinish()
{
	//D_BPFinishBlend.Execute();
	Bomb->DestroyComponent();
}

void AInteractStatue::SetMassBlendTimelineUpdate(float Value)
{
	BlendRadius = BombDistance * Value;

	PCI->SetScalarParameterValue(FName("MassRadius"), BlendRadius);
	Bomb->SetNiagaraVariableVec3("Scale Factor", { BlendRadius / 50, BlendRadius / 50, BlendRadius / 50 });
	WindNS->SetNiagaraVariableFloat("Radius", BlendRadius);
}

void AInteractStatue::SetNormalAmplifyTimelineUpdate(float Value)
{
	PCI->SetScalarParameterValue(FName("MassAmplify"), Value);
	UE_LOG(LogTemp, Warning, TEXT("%f"),Value);
}

void AInteractStatue::SetEmissiveTimelineUpdate(float Value)
{
	PCI->SetVectorParameterValue(FName("MassEmissiveColor"), Value * InitialEmissiveColor);
}

//==================BlendMassTimeline====================
//==================BlendMassTimeline====================
//==================BlendMassTimeline====================

//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================
void AInteractStatue::SetShakeSMTimelineFinish()
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
void AInteractStatue::SetShakeSMTimelineUpdate(float Value)
{
		AddActorLocalOffset({ sin(double(Value) * 10000) * 3 ,sin(double(Value) * 10000) * 3 ,Value});
		for (UMaterialInstanceDynamic* a : ShakeSMDMIList) {
			FLinearColor TempLinear = {0.f, 0.f, (Value * 5000.f + 1.f), 1.0f};
			a->SetVectorParameterValue(FName("EmissiveColor"), TempLinear);
			a->SetScalarParameterValue(FName("PannerSpeed"), Value * 10 + 1.0f);
		}
}
void AInteractStatue::SetSizeSMTimelineUpdate(float Value)
{
	StaticMesh->SetWorldScale3D({ Value,Value,Value });
}

//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================
//==================ShakeSMTimeline====================


// Called every frame
void AInteractStatue::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}
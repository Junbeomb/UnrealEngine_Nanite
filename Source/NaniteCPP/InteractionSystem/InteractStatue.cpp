// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractStatue.h"

#include "Components/SphereComponent.h"
#include "InstancedFoliageActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AInteractStatue::AInteractStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	CheckInstanceSMC = CreateDefaultSubobject<USphereComponent>(TEXT("CheckISMC"));
	CheckInstanceSMC->SetupAttachment(RootComponent);
	CheckInstanceSMC->SetSphereRadius(0.f);

	Bomb = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Bomb"));
	Bomb->SetupAttachment(RootComponent);

	WindNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindNS"));
	WindNS->SetupAttachment(RootComponent);

	CompBase = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_InteractBase"));

	Bomb->SetAutoActivate(false);
	WindNS->SetAutoActivate(false);

	BombDistance = 2000.f;

	//MassBlendTimeline
	MassBlendTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MassBlendTimeline"));
	MassBlendTimelineFinishedCallback.BindUFunction(this, FName("SetMassBlendTimelineFinish"));
	MassBlendTimelineUpdateCallback.BindUFunction(this, FName("SetMassBlendTimelineUpdate"));
	NormalAmplifyTimelineUpdateCallback.BindUFunction(this, FName("SetNormalAmplifyTimelineUpdate"));
	EmissiveTimelineUpdateCallback.BindUFunction(this, FName("SetEmissiveTimelineUpdate"));

	//SMCSPhereTimeline
	SMCSphereTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SMCSPhereTImeline"));
	SMCSphereTimelineFinishedCallback.BindUFunction(this, FName("SetSMCSphereTimelineFinish"));
	SMCSphereTimelineUpdateCallback.BindUFunction(this, FName("SetSMCSPhereTimelineUpdate"));

}

void AInteractStatue::PressEStart()
{
	UE_LOG(LogTemp, Warning, TEXT("PressEStart"));
	CompBase->TurnOffHover();
	CompBase->DestroyComponent();

	//월드내의 Foliage Static Mesh 리스트 가져오기
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), Temp);
	Temp[0]->GetComponents<UInstancedStaticMeshComponent>(InstancedMesh);

	//DMI로 변경, 변수 변경(바뀔준비)
	for (UInstancedStaticMeshComponent* a : InstancedMesh) {
		if (a->GetStaticMesh()->GetName().Contains("SM_L_")) {
			for (int i = 0; i < a->GetNumMaterials(); i++) {
				UMaterialInstanceDynamic* TempDMI = a->CreateDynamicMaterialInstance(i, a->GetMaterial(i));
				TempDMI->SetScalarParameterValue("IsMassBlending?", 1.0f);
				TempDMI->SetScalarParameterValue("IsChanging?", 1.0f);
				TempDMI->SetVectorParameterValue("MassCenter", GetActorLocation());

				DMIList.Add(TempDMI);
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *a->GetStaticMesh()->GetName());
	}

	//NiagaraComponent 세팅
	Bomb->SetAutoActivate(true);
	WindNS->SetAutoActivate(true);

	Bomb->SetNiagaraVariableFloat("Lifetime", BombDistance / 1000); //1초당 1000cm만큼 이동하므로
	WindNS->SetNiagaraVariableVec2("PersonLocation", { GetActorLocation().X,GetActorLocation().Y });
	WindNS->SetNiagaraVariableInt("Resolution", 256);
	WindNS->SetNiagaraVariableFloat("SimulationSize", 4096.f);

	StaticMesh->DestroyComponent();

	StartMassBlend();
}

void AInteractStatue::StartMassBlend()
{
	//UE_LOG(LogTemp, Warning, TEXT("startMassBlend"));

	//D_BPStartBlend.Execute();

	PCI = GetWorld()->GetParameterCollectionInstance(ParamCollection);

	if (PCI) {
		FLinearColor EColor;
		PCI->GetVectorParameterValue(FName("EmissiveColor"), EColor);
	}

	//MassBlendTimeline
	if (MassBlendCurve && NormalAmplifyCurve && EmissiveCurve) {
		MassBlendTimeline->AddInterpFloat(MassBlendCurve, MassBlendTimelineUpdateCallback, FName("Blend"));
		MassBlendTimeline->AddInterpFloat(NormalAmplifyCurve, NormalAmplifyTimelineUpdateCallback, FName("NormalAmplify"));
		MassBlendTimeline->AddInterpFloat(EmissiveCurve, EmissiveTimelineUpdateCallback, FName("Emissive"));

		MassBlendTimeline->SetTimelineFinishedFunc(MassBlendTimelineFinishedCallback);

		MassBlendTimeline->PlayFromStart();
	}

	if (SMCSphereCurve) {
		SMCSphereTimeline->AddInterpFloat(SMCSphereCurve, SMCSphereTimelineUpdateCallback, FName("SphereRadius"));
		SMCSphereTimeline->SetTimelineFinishedFunc(SMCSphereTimelineFinishedCallback);
		SMCSphereTimeline->PlayFromStart();
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
	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetScalarParameterValue("MassRadius", BlendRadius);
	}
	Bomb->SetNiagaraVariableVec3("Scale Factor", FVector(BlendRadius / 50));
	WindNS->SetNiagaraVariableFloat("Radius", BlendRadius);
}

void AInteractStatue::SetNormalAmplifyTimelineUpdate(float Value)
{

	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetScalarParameterValue("MassVertexNormalAmplify", Value);
	}
	PCI->SetScalarParameterValue("MassVertexNormalAmplify", Value);
}

void AInteractStatue::SetEmissiveTimelineUpdate(float Value)
{
	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetVectorParameterValue("EmissiveColor", Value * InitialEmissiveColor);
	}
	PCI->SetVectorParameterValue("EmissiveColor", Value * InitialEmissiveColor);
}
//==================BlendMassTimeline====================
//==================BlendMassTimeline====================
//==================BlendMassTimeline====================

//==================SMCSphereTimeline====================
//==================SMCSphereTimeline====================
//==================SMCSphereTimeline====================
void AInteractStatue::SetSMCSphereTimelineFinish()
{
	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetScalarParameterValue("IsMassBlending?", 0.0f);
		DMI->SetScalarParameterValue("IsChanging?", 0.0f);
		DMIList.Remove(DMI);
	}

}
void AInteractStatue::SetSMCSphereTimelineUpdate(float Value)
{
	CheckInstanceSMC->SetSphereRadius(Value * BombDistance);
}
//==================SMCSphereTimeline====================
//==================SMCSphereTimeline====================
//==================SMCSphereTimeline====================


// Called when the game starts or when spawned
void AInteractStatue::BeginPlay()
{
	Super::BeginPlay();
	CompBase->SetDynamicMaterial(StaticMesh, false);
}

// Called every frame
void AInteractStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


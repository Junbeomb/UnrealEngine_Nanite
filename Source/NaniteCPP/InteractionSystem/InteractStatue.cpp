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

	CheckInstanceSMC = CreateDefaultSubobject<USphereComponent>(TEXT("CheckInstanceSMC"));
	CheckInstanceSMC->SetupAttachment(RootComponent);
	CheckInstanceSMC->SetSphereRadius(0.f);
	CheckInstanceSMC->SetHiddenInGame(false);
	//CheckInstanceSMC�� ��ġ��
	CheckInstanceSMC->OnComponentBeginOverlap.AddDynamic(this, &AInteractStatue::OverlapSMCRange);

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

	//ShakeSMTimeline
	ShakeSMTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ShakeSMTimeline"));
	ShakeSMTimelineFinishedCallback.BindUFunction(this, FName("SetShakeSMTimelineFinish"));
	ShakeSMTimelineUpdateCallback.BindUFunction(this, FName("SetShakeSMTImelineUpdate"));
	SizeSMTimelineUpdateCallback.BindUFunction(this, FName("SetSizeSMTImelineUpdate"));

}

void AInteractStatue::PressEStart()
{
	UE_LOG(LogTemp, Warning, TEXT("PressEStart"));
	CompBase->TurnOffHover();
	CompBase->DestroyComponent();

	//���峻�� Foliage Static Mesh ����Ʈ ��������
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), Temp);
	Temp[0]->GetComponents<UInstancedStaticMeshComponent>(InstancedMesh);

	//DMI�� ����, ���� ����(�ٲ��غ�)
	for (UInstancedStaticMeshComponent* a : InstancedMesh) {
		if (a->GetStaticMesh()->GetName().Contains("SM_L_")) {
			for (int i = 0; i < a->GetNumMaterials(); i++) {
				UMaterialInstanceDynamic* TempDMI = a->CreateDynamicMaterialInstance(i, a->GetMaterial(i));
				TempDMI->SetScalarParameterValue(FName("IsMassBlending?"), 1.0f);
				TempDMI->SetScalarParameterValue(FName("IsChanging?"), 1.0f);
				TempDMI->SetVectorParameterValue(FName("MassCenter"), GetActorLocation());

				DMIList.Add(TempDMI);
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *a->GetStaticMesh()->GetName());
	}


	//Statue StaticMesh �����̰��ϱ�
	for (int i = 0; i < StaticMesh->GetNumMaterials(); i++) {
		UMaterialInstanceDynamic* MID = StaticMesh->CreateDynamicMaterialInstance(i, StaticMesh->GetMaterial(i));
		ShakeSMDMIList.Add(MID);
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

	//D_BPStartBlend.Execute();

	PCI = GetWorld()->GetParameterCollectionInstance(ParamCollection);

	if (PCI) {
		FLinearColor EColor;
		PCI->GetVectorParameterValue(FName("EmissiveColor"), EColor);
	}

	if (SMCSphereCurve) {
		SMCSphereTimeline->AddInterpFloat(SMCSphereCurve, SMCSphereTimelineUpdateCallback, FName("SphereRadius"));
		SMCSphereTimeline->SetTimelineFinishedFunc(SMCSphereTimelineFinishedCallback);
		SMCSphereTimeline->PlayFromStart();
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
	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetScalarParameterValue(FName("MassRadius"), BlendRadius);
	}
	Bomb->SetNiagaraVariableVec3("Scale Factor", { BlendRadius / 50, BlendRadius / 50, BlendRadius / 50 });
	WindNS->SetNiagaraVariableFloat("Radius", BlendRadius);
}

void AInteractStatue::SetNormalAmplifyTimelineUpdate(float Value)
{

	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetScalarParameterValue(FName("MassVertexNormalAmplify"), Value);
	}
	PCI->SetScalarParameterValue(FName("MassVertexNormalAmplify"), Value);
}

void AInteractStatue::SetEmissiveTimelineUpdate(float Value)
{
	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetVectorParameterValue(FName("EmissiveColor"), Value * InitialEmissiveColor);
	}
	PCI->SetVectorParameterValue(FName("EmissiveColor"), Value * InitialEmissiveColor);
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

	//NiagaraComponent ����
	Bomb->Activate(true);
	WindNS->Activate(true);

	Bomb->SetNiagaraVariableFloat("Lifetime", BombDistance / 1000); //1�ʴ� 1000cm��ŭ �̵��ϹǷ�
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


//==================SMCSphereTimeline====================
//==================SMCSphereTimeline====================
//==================SMCSphereTimeline====================
void AInteractStatue::SetSMCSphereTimelineFinish()
{
	for (UMaterialInstanceDynamic* DMI : DMIList) {
		DMI->SetScalarParameterValue(FName("IsMassBlending?"), 0.0f);
		DMI->SetScalarParameterValue(FName("IsChanging?"), 0.0f);
		DMIList.Remove(DMI);
	}

	Destroy();
}
void AInteractStatue::SetSMCSphereTimelineUpdate(float Value)
{
	CheckInstanceSMC->SetSphereRadius(Value * BombDistance);
}

//��ġ��
void AInteractStatue::OverlapSMCRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//Overlap �̺�Ʈ�� �߻����� ����.

	UInstancedStaticMeshComponent* TempISMC = Cast<UInstancedStaticMeshComponent>(SweepResult.GetComponent());
	if (TempISMC) {
		FString TempName = TempISMC->GetStaticMesh()->GetName();
		if (TempName.Contains("SM_L_")) {
			int32 TempHitItem = SweepResult.Item;

			//Add UpScore!!

			FTransform TempTransform;
			TempISMC->GetInstanceTransform(TempHitItem, TempTransform, true);

			FString RightS;
			TempName.Split("SM_L_", NULL, &RightS);
			RightS = "SM_H_" + RightS;

			for (UInstancedStaticMeshComponent* a : InstancedMesh) {
				//UE_LOG(LogTemp, Warning, TEXT("%s"),*a->GetStaticMesh()->GetName());
				if (a->GetStaticMesh()->GetName().Contains(RightS)) {
					a->AddInstance(TempTransform,true);
				}
			}

			//Add SpawnNiagara

			TempISMC->RemoveInstance(TempHitItem);
		}
	}
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


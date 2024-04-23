// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractStatue.h"

#include "InstancedFoliageActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AInteractStatue::AInteractStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//BombNS->CreateDefaultSubobject<UNiagaraComponent>(TEXT("BombNS"));
	//BombNS->SetupAttachment(StaticMesh);

	//WindNS->CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindNS"));
	//WindNS->SetupAttachment(StaticMesh);

	CompBase = CreateDefaultSubobject<UComp_InteractBase>(TEXT("Comp_InteractBase"));

	//BombNS->Activate(false);
	//WindNS->Activate(false);
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

	/*for (UMaterialInstanceDynamic* temp : DMIList) {
		UE_LOG(LogTemp, Warning, TEXT("%s"),*temp->GetName());
	}*/

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BombNS, GetActorLocation());
	StaticMesh->DestroyComponent();

	StartMassBlend();
}

void AInteractStatue::StartMassBlend()
{
	UE_LOG(LogTemp, Warning, TEXT("startMassBlend"));
}

// Called when the game starts or when spawned
void AInteractStatue::BeginPlay()
{
	Super::BeginPlay();
	
	CompBase->SetDynamicMaterial(StaticMesh, false);

	UE_LOG(LogTemp, Warning, TEXT("asdf"));
}

// Called every frame
void AInteractStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


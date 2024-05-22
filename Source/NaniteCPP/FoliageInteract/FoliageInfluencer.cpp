// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageInfluencer.h"
#include "Kismet/GameplayStatics.h"
#include "FoliageHelpProjectile.h"
#include "FoliagePlantBase.h"
#include "FoliageRockBase.h"
#include "../Blending/Comp_BlendMesh.h"
#include "../Blending/MaterialChangeBall.h"
#include "../Blackhole/BlackholeLightBase.h"
#include "../Blackhole/BlackholeHeavyBase.h"
#include <string.h>

#include "Components/InstancedStaticMeshComponent.h"
//#include "Containers/UnrealString.h"

// Sets default values
AFoliageInfluencer::AFoliageInfluencer()
{
	PrimaryActorTick.bCanEverTick = true;

	//��Ʈ ������Ʈ ���� �� �Ž�������Ʈ ����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	FoliageTransMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoliageTransMesh"));
	FoliageTransMesh->SetupAttachment(RootComponent);

	PhysicsRadius = 0.f;
	IsBlackholeInfluencer = false;
}


void AFoliageInfluencer::SpawnAndConvert(UMeshComponent* TempMesh)
{
	FString Right;
	value.Split("SM_", NULL, &Right);

	int NumMaterial = TempMesh->GetNumMaterials();

	if (Right.Contains("L_")) {
		for (int i = 0; i < NumMaterial; ++i) {
			UMaterialInstanceDynamic* TempDMI = TempMesh->CreateDynamicMaterialInstance(i, TempMesh->GetMaterial(i));
			TempDMI->SetScalarParameterValue("IsLow?", 1.0f);
		}
	}
	else if (Right.Contains("H_")) {
		for (int i = 0; i < NumMaterial; ++i) {
			UMaterialInstanceDynamic* TempDMI = TempMesh->CreateDynamicMaterialInstance(i, TempMesh->GetMaterial(i));
			TempDMI->SetScalarParameterValue("IsLow?", 0.0f);
		}
	}
}

void AFoliageInfluencer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	bool isHit = FindTrace();

	for (FHitResult& Hit : OutResults) {
		if (Hit.bBlockingHit) {
			if (IsBlackholeInfluencer) { //��Ȧ ����Ʈ���� ã��
				InstancedMeshComp = Cast<UInstancedStaticMeshComponent>(Hit.GetComponent());

				if (InstancedMeshComp) {
						
					InstancedMeshComp->GetInstanceTransform(Hit.Item, InstanceTransform, true);

					value = *InstancedMeshComp->GetStaticMesh()->GetName();
					//����Ʈ���� �̸� �˻�

					if (BlackholeFoliageBlueprints.Num() > 0) {
						for (const TSubclassOf<AActor> FoliageBP : BlackholeFoliageBlueprints) {
							bool isContain = value.Contains(*FoliageBP->GetName().RightChop(6).LeftChop(2), ESearchCase::IgnoreCase, ESearchDir::FromEnd);

							//�迭�� �ش� SM->BP �� ������
							if (isContain) {

								ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

								AActor* SpawnBP = GetWorld()->SpawnActor<AActor>(FoliageBP, InstanceTransform, ActorSpawnParams);

								//High,Low ����Ȯ��====================================================
								//High,Low ����Ȯ��====================================================
								//High,Low ����Ȯ��====================================================
								ABlackholeLightBase* PlantBase = Cast<ABlackholeLightBase>(SpawnBP);
								if (PlantBase != nullptr) {
									UStaticMeshComponent* SM = PlantBase->BaseStaticMesh;
									SpawnAndConvert(SM);
								}
								else {
									ABlackholeHeavyBase* RockBase = Cast<ABlackholeHeavyBase>(SpawnBP);
									if (RockBase != nullptr) {
										UStaticMeshComponent* SM = RockBase->BaseStaticMesh;
										SpawnAndConvert(SM);
									}
								}
								//High,Low ����Ȯ��===================================================
								//High,Low ����Ȯ��===================================================
								//High,Low ����Ȯ��===================================================

								InstancedMeshComp->RemoveInstance(Hit.Item);
							}
						}
					}
				}
			}
			else { //�Ϲ� BP ����Ʈ���� ã��
				InstancedMeshComp = Cast<UInstancedStaticMeshComponent>(Hit.GetComponent());

				if (InstancedMeshComp) {
					InstancedMeshComp->GetInstanceTransform(Hit.Item, InstanceTransform, true);

					value = *InstancedMeshComp->GetStaticMesh()->GetName();
					//����Ʈ���� �̸� �˻�

					if (FoliageBlueprints.Num() > 0) {
						for (const TSubclassOf<AActor> FoliageBP : FoliageBlueprints) {
							bool isContain = value.Contains(*FoliageBP->GetName().RightChop(3).LeftChop(2), ESearchCase::IgnoreCase, ESearchDir::FromEnd);

							//�迭�� �ش� SM->BP �� ������
							if (isContain) {
								ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

								AActor* SpawnBP = GetWorld()->SpawnActor<AActor>(FoliageBP, InstanceTransform, ActorSpawnParams);

								//High,Low ����Ȯ��====================================================
								//High,Low ����Ȯ��====================================================
								//High,Low ����Ȯ��====================================================
								AFoliagePlantBase* PlantBase = Cast<AFoliagePlantBase>(SpawnBP);
								if (PlantBase != nullptr) {
									USkeletalMeshComponent* SK = PlantBase->MeshComponent;
									SpawnAndConvert(SK);

									//���� ������Ʈ ���� �˻� �� �Լ� ����
									CheckBlend(PlantBase, Hit.Location);
								}
								else {
									AFoliageRockBase* RockBase = Cast<AFoliageRockBase>(SpawnBP);
									if (RockBase != nullptr) {
										UStaticMeshComponent* SM = RockBase->MeshComponent;
										SpawnAndConvert(SM);
										//���� ������Ʈ ���� �˻� �� �Լ� ����
										CheckBlend(RockBase, Hit.Location);
									}
								}
								//High,Low ����Ȯ��===================================================
								//High,Low ����Ȯ��===================================================
								//High,Low ����Ȯ��===================================================
								InstancedMeshComp->RemoveInstance(Hit.Item);
							}
						}
					}
				}
			}
		}
	}
}

bool AFoliageInfluencer::FindTrace()
{
	UChildActorComponent* Parentcomp = GetParentComponent();

	if (Parentcomp) {
		FVector StartLocation = Parentcomp->GetComponentLocation();

		FCollisionShape FoliageSphere = FCollisionShape::MakeSphere(PhysicsRadius);

		//DrawDebugSphere(GetWorld(), StartLocation, FoliageSphere.GetSphereRadius(), 10, FColor::Purple, false, 0.1f);

		return GetWorld()->SweepMultiByChannel(OutResults, StartLocation, StartLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FoliageSphere);

	}
	else {
		AActor* ParentActor = GetParentActor();

		if (ParentActor) {
			FVector StartLocation = ParentActor->GetActorLocation();

			FCollisionShape FoliageSphere = FCollisionShape::MakeSphere(PhysicsRadius);

			//�����
			//DrawDebugSphere(GetWorld(), StartLocation, FoliageSphere.GetSphereRadius(), 10, FColor::Purple, false, 0.1f);

			return GetWorld()->SweepMultiByChannel(OutResults, StartLocation, StartLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FoliageSphere);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Cannot detected parent component!!!!"));

			return false;
		}
	}

	return false;
}

void AFoliageInfluencer::CheckBlend(AActor* CheckActor, FVector ImpactPoint)
{
	UComp_BlendMesh* UBM = Cast<UComp_BlendMesh>(CheckActor->GetComponentByClass(UComp_BlendMesh::StaticClass()));

	if (IsValid(CheckActor->GetComponentByClass(UComp_BlendMesh::StaticClass()))) {
		switch (FoliageType) {
		case EFoliageType::JustGo:
			UBM->JustGo();
			break;
		case EFoliageType::ChangeBlend:
			if (!UBM->IsBlendStart) {
				if (MaterialChangeBall) {
					FVector Origin;
					FVector BoxExtent;
					CheckActor->GetActorBounds(false, Origin, BoxExtent, false);
					FTransform TempTransform = { {0,0,0}, ImpactPoint ,BoxExtent / 100};
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					GetWorld()->SpawnActor<AMaterialChangeBall>(MaterialChangeBall, TempTransform, ActorSpawnParams);
					UBM->StartBlend();
				}
			}
			break;
		}
	}
}




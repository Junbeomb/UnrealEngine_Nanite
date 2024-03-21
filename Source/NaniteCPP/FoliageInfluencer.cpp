// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageInfluencer.h"
#include "Kismet/GameplayStatics.h"
#include "FoliageHelpProjectile.h"

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

	PhysicsRadius = 500.f;
}

void AFoliageInfluencer::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<AFoliageHelpProjectile> ClassToFind;
	ClassToFind = AFoliageHelpProjectile::StaticClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

	if (FoundActors.Num() == 0) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		FTransform transform;
		transform.SetScale3D({ 1,1,1 });
		GetWorld()->SpawnActor<AFoliageHelpProjectile>(AFoliageHelpProjectile::StaticClass(), transform, ActorSpawnParams);
	}
	
}

void AFoliageInfluencer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	bool isHit = FindTrace();

	for (const FHitResult& Hit : OutResults) {
		if (Hit.bBlockingHit) {
			UInstancedStaticMeshComponent* InstancedMeshComp = Cast<UInstancedStaticMeshComponent>(Hit.GetComponent());

			if (InstancedMeshComp) {
				FTransform InstanceTransform;
				InstancedMeshComp->GetInstanceTransform(Hit.Item, InstanceTransform, true);

				FString value = *InstancedMeshComp->GetStaticMesh()->GetName();
				//����Ʈ���� �̸� �˻�
				if (FoliageBlueprints.Num() > 0) {
					for (const TSubclassOf<AActor> FoliageBP : FoliageBlueprints) {
						bool isContain = value.Contains(*FoliageBP->GetName().RightChop(3).LeftChop(2), ESearchCase::IgnoreCase, ESearchDir::FromEnd);

						//�迭�� �ش� SM->BP �� ������
						if (isContain) {

							FActorSpawnParameters ActorSpawnParams;
							ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

							GetWorld()->SpawnActor<AActor>(FoliageBP, InstanceTransform, ActorSpawnParams);
							InstancedMeshComp->RemoveInstance(Hit.Item);
						}
						//UE_LOG(LogTemp, Warning, TEXT("%d"), isContain);
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

		DrawDebugSphere(GetWorld(), StartLocation, FoliageSphere.GetSphereRadius(), 10, FColor::Purple, false, 0.1f);

		return GetWorld()->SweepMultiByChannel(OutResults, StartLocation, StartLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FoliageSphere);

	}
	else {
		AActor* ParentActor = GetParentActor();

		if (ParentActor) {
			FVector StartLocation = ParentActor->GetActorLocation();

			FCollisionShape FoliageSphere = FCollisionShape::MakeSphere(PhysicsRadius);

			DrawDebugSphere(GetWorld(), StartLocation, FoliageSphere.GetSphereRadius(), 10, FColor::Purple, false, 0.1f);

			return GetWorld()->SweepMultiByChannel(OutResults, StartLocation, StartLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FoliageSphere);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Cannot detected parent component!!!!"));

			return false;
		}
	}

	return false;
}

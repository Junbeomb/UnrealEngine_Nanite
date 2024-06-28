#include "MeteorRock.h"
#include "MeteorChargeCenter.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorSoulBall.h"


AMeteorRock::AMeteorRock()
{
	PrimaryActorTick.bCanEverTick = true;

	MeteorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeteorMesh"));
	RootComponent = MeteorMesh;
}


void AMeteorRock::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeteorRock::MeteorCenterBind()
{
	Center = Cast<AMeteorChargeCenter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMeteorChargeCenter::StaticClass()));
	//Center = Cast<AMeteorChargeCenter>(AMeteorChargeCenter::StaticClass());
	if(Center)
		Center->D_AbsortStart.AddUObject(this, &AMeteorRock::AbsorbedByCenter);
}

void AMeteorRock::AbsorbedByCenter()
{
	UE_LOG(LogTemp, Warning, TEXT("AbsorbedByCenter"));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AMeteorSoulBall>(SoulBallChoice, GetActorLocation(), {0,0,0}, ActorSpawnParams);

	Destroy();
}

// Called every frame
void AMeteorRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


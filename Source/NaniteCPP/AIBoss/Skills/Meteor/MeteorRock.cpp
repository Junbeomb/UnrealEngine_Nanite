
#include "MeteorRock.h"
#include "Kismet/GameplayStatics.h"

#include "MeteorChargeCenter.h"
#include "MeteorSoulBall.h"
#include "../../../Blending/Comp_BlendMesh.h"


AMeteorRock::AMeteorRock()
{
	PrimaryActorTick.bCanEverTick = true;

	MeteorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeteorMesh"));
	RootComponent = MeteorMesh;

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));

	//사라지는 Timeline
	DissapearTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissapearTimeline"));
	DissapearTimelineCallback.BindUFunction(this, FName("DissapearTimelineUpdate"));
	DissapearTimelineFinishedCallback.BindUFunction(this, FName("DissapearTimelineFinish"));
}


void AMeteorRock::BeginPlay()
{
	Super::BeginPlay();

	Comp_Blend->D_StartBlending.BindUObject(this, &AMeteorRock::MeteorRockBlendStart);
	Comp_Blend->D_FinishBlending.BindUObject(this, &AMeteorRock::MeteorRockBlendFinish);

	DMI = MeteorMesh->CreateDynamicMaterialInstance(0, MeteorMesh->GetMaterial(0));
}

void AMeteorRock::MeteorRockBlendStart()
{
	UE_LOG(LogTemp, Warning, TEXT("MeteorRockBlendStart"));
	IsBlendStart = true;
}

void AMeteorRock::MeteorRockBlendFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("MeteorRockBlendFinish"));
}

void AMeteorRock::MeteorCenterBind()
{
	if (IsBlendStart) return;

	Center = Cast<AMeteorChargeCenter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMeteorChargeCenter::StaticClass()));
	//Center = Cast<AMeteorChargeCenter>(AMeteorChargeCenter::StaticClass());
	if(Center)
		Center->D_AbsortStart.AddUObject(this, &AMeteorRock::AbsorbedByCenter);
}

void AMeteorRock::AbsorbedByCenter()
{
	//UE_LOG(LogTemp, Warning, TEXT("AbsorbedByCenter"));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AMeteorSoulBall>(SoulBallChoice, GetActorLocation(), {0,0,0}, ActorSpawnParams);
	

	if (DissapearCurve) {
		DissapearTimeline->AddInterpFloat(DissapearCurve, DissapearTimelineCallback, FName("Dissapear"));
		DissapearTimeline->SetTimelineFinishedFunc(DissapearTimelineFinishedCallback);
		DissapearTimeline->SetPlayRate(0.2f);
		DissapearTimeline->PlayFromStart();
	}
	
}

void AMeteorRock::DissapearTimelineUpdate(float Value)
{
	if(DMI)
		DMI->SetScalarParameterValue(FName("Progress"), Value*3.f);
}
void AMeteorRock::DissapearTimelineFinish()
{
	Destroy();
}

// Called every frame
void AMeteorRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


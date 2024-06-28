#include "HomingBallBase.h"


#include "BossDefaults/BossBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AHomingBallBase::AHomingBallBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereComp;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(SphereComp);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(SphereComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	//SphereComp ¶û ºÎµúÈ÷¸é
	SphereComp->OnComponentHit.AddDynamic(this, &AHomingBallBase::OnComponentHit);

}

void AHomingBallBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnComponentHit!!"));
	/*ANaniteCPPCharacter* tempCharac = Cast<ANaniteCPPCharacter>(OtherActor);
	tempCharac->DecreaseHP();*/
	Destroy();
	//D_Hit.Broadcast();
}


void AHomingBallBase::BeginPlay()
{
	Super::BeginPlay();
	
	//ProjectileMovement->HomingTargetComponent = UGameplayStatics::GetActorOfClass(GetWorld(), ABOssTest::StaticClass())->GetRootComponent();
}

void AHomingBallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


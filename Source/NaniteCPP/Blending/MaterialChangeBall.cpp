// Fill out your copyright notice in the Description page of Project Settings.


#include "Comp_BlendMesh.h"
#include "MaterialChangeBall.h"

// Sets default values
AMaterialChangeBall::AMaterialChangeBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TickStart = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChangeBallMesh"));
	RootComponent = StaticMesh;

	CompBlend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_BlendMesh"));
}

// Called when the game starts or when spawned
void AMaterialChangeBall::BeginPlay()
{
	Super::BeginPlay();
	
	InitialScale = StaticMesh->GetRelativeScale3D();
	
	//StaticMesh->SetWorldScale3D({ 0,0,0 });
	TickStart = true;
}

// Called every frame
void AMaterialChangeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TickStart) {
		CountTick += DeltaTime;

		if (CountTick > 1.0) {
			//UE_LOG(LogTemp, Warning, TEXT("%f"), InitialScale.GetMax() / (CompBlend->ExtentSubtractAmountOneSecond / 100));
			//Delay -> 6�ʵ� Mesh�پ��� Animation����
			FTimerHandle DestroyTimerHandle;
			FTimerDelegate TimerDelegate;

			TimerDelegate.BindLambda([&]
			{
				Destroy();
			});

			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, InitialScale.GetMax()/ (CompBlend->ExtentSubtractAmountOneSecond/100), false); //Actor�� ũ�⸸ŭ ������

		}

		//StaticMesh->SetWorldScale3D((InitialScale/2) * CountTick);

	}
}


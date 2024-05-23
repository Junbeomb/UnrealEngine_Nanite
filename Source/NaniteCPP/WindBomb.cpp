// Fill out your copyright notice in the Description page of Project Settings.


#include "WindBomb.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWindBomb::AWindBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}



// Called when the game starts or when spawned
void AWindBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWindBomb::BlackholeFunc()
{
	//windmanager 가져오기
	//APuzzleKeyDoor* puzzleDoor = Cast<APuzzleKeyDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleKeyDoor::StaticClass()));
}




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialChangeBall.generated.h"

UCLASS()
class NANITECPP_API AMaterialChangeBall : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	FVector InitialScale;
	class UComp_BlendMesh* CompBlend;

	bool TickStart;
	float CountTick;

	
public:	
	// Sets default values for this actor's properties
	AMaterialChangeBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

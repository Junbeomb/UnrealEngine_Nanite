// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleSwitchBox.generated.h"

DECLARE_MULTICAST_DELEGATE(FD_SwitchBox);
UCLASS()
class NANITECPP_API APuzzleSwitchBox : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere)
	class UComp_BlendMesh* Comp_Blend;

	UFUNCTION()
	void OnFinishBlending();
	
public:	
	// Sets default values for this actor's properties
	APuzzleSwitchBox();

	FD_SwitchBox SwitchBoxOn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

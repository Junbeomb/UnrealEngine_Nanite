// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../InteractionSystem/Interface_Interact.h"
#include "PuzzleKey.generated.h"

UCLASS()
class NANITECPP_API APuzzleKey : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* KeyStaticMesh;

	UPROPERTY(EditAnywhere)
	class UComp_BlendMesh* Comp_Blend;

	UPROPERTY(EditAnywhere)
	class UComp_InteractBase* Comp_Interact;

	UFUNCTION()
	void OnFinishBlending();
	
public:	
	// Sets default values for this actor's properties
	APuzzleKey();

	virtual void PressEStart() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

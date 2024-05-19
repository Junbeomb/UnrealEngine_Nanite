// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../InteractionSystem/Interface_Interact.h"
#include "PuzzleKeyPad.generated.h"

UCLASS()
class NANITECPP_API APuzzleKeyPad : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* KeyPadMesh;

	UPROPERTY(EditAnywhere)
	class UComp_BlendMesh* Comp_Blend;


	UPROPERTY(EditAnywhere)
   class UComp_InteractBase* Comp_Interact;


	UFUNCTION()
	void OnFinishBlending();

public:	
	// Sets default values for this actor's properties
	APuzzleKeyPad();

	UPROPERTY(EditAnywhere)
	bool isGetKey;

	virtual void PressEStart() override;

	UFUNCTION()
	void GainKey();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

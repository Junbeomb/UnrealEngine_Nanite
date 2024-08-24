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

	UPROPERTY()
	class UComp_BlendMesh* Comp_Blend;

	UPROPERTY()
   class UComp_InteractBase* Comp_Interact;

   bool isGetKey;
	void OnFinishBlending();

public:	
	APuzzleKeyPad();
	virtual void PressEStart() override;

	void GainKey();
};

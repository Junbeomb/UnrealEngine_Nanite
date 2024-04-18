// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Interact.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Interact : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NANITECPP_API IInterface_Interact
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	
	virtual void PressEStart();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hello")
	bool GetSlowHoverEffect();
};

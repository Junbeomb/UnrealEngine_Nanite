#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaniteCPP/BeginnerCharacter/NaniteCPPCharacter.h"
#include "Comp_BlendMesh.generated.h"

DECLARE_DELEGATE(FD_Blend);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NANITECPP_API UComp_BlendMesh : public UActorComponent
{
	GENERATED_BODY()

	void FinishBlendSetVariable();

	ANaniteCPPCharacter* Player;

	UStaticMeshComponent* SMC;
	USkeletalMeshComponent* SKC;

	TArray<UMaterialInstanceDynamic*> DMIList;

	bool IsHighQuality;

	bool OwnerIsStatic;

	float SumSeconds;

	float WhichOneIsLongestXYZ;
	float ExtentSubtractAmountOneSecond;

	bool IsBlendStart;

	UMeshComponent* StaticOrSkeletal();

	void CreateDMIAndDFOff(UPrimitiveComponent* UComp, int NumMaterial);

protected:
	virtual void BeginPlay() override;

public:	
	UComp_BlendMesh();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartBlend();

	void JustGo();

	bool IsLow();

	bool GetIsBlendStart();

	FD_Blend D_FinishBlending;
	FD_Blend D_StartBlending;
	FD_Blend D_JustGo;

};

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
	//알아서 메모리 관리
	 //AddComponentByClass 로 값을 초기화 하므로 UPROPERTY() 안붙여도 됨. PuzzleKeyPad에 사용된 NewOBject는 붙여야됨.
	UPROPERTY() 
	class UComp_InteractBase* Comp_Interact;
	//CreateDefaultSubobject 로 값을 초기화 하므로 UPROPERTY() 안붙여도 됨.
	class UComp_BlendMesh* Comp_Blend; 


	//UFUNCTION()을 붙이지 않아도 됨.
	//타임라인의 Binding과는 다르게 C++에서 바인드 하는 함수이므로
	//타임라인의 Binding는 언리얼 엔진의 특수기능이라서 UFUNCTION() 을 붙여주어야 Binding 될 수 있음. (리플렉션 시스템)
	void OnFinishBlending();

public:
	APuzzleKey();

	virtual void PressEStart() override;


};

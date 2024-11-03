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
	//�˾Ƽ� �޸� ����
	 //AddComponentByClass �� ���� �ʱ�ȭ �ϹǷ� UPROPERTY() �Ⱥٿ��� ��. PuzzleKeyPad�� ���� NewOBject�� �ٿ��ߵ�.
	UPROPERTY() 
	class UComp_InteractBase* Comp_Interact;
	//CreateDefaultSubobject �� ���� �ʱ�ȭ �ϹǷ� UPROPERTY() �Ⱥٿ��� ��.
	class UComp_BlendMesh* Comp_Blend; 


	//UFUNCTION()�� ������ �ʾƵ� ��.
	//Ÿ�Ӷ����� Binding���� �ٸ��� C++���� ���ε� �ϴ� �Լ��̹Ƿ�
	//Ÿ�Ӷ����� Binding�� �𸮾� ������ Ư������̶� UFUNCTION() �� �ٿ��־�� Binding �� �� ����. (���÷��� �ý���)
	void OnFinishBlending();

public:
	APuzzleKey();

	virtual void PressEStart() override;


};

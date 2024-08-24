// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleKeyPad.h"
#include "../../Blending/Comp_BlendMesh.h"
#include "../../InteractionSystem/Comp_InteractBase.h"
#include "PuzzleKeyDoor.h"
#include "Kismet/GameplayStatics.h"

APuzzleKeyPad::APuzzleKeyPad()
{
	KeyPadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyPadMesh"));
	RootComponent = KeyPadMesh;

	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));
	Comp_Blend->D_FinishBlending.BindUObject(this, &APuzzleKeyPad::OnFinishBlending);

	isGetKey = false;
}

//blend�� ������ ��ȣ�ۿ� Ȱ��ȭ
void APuzzleKeyPad::OnFinishBlending()
{
	Comp_Interact = NewObject<UComp_InteractBase>(this, UComp_InteractBase::StaticClass(), NAME_None, RF_Transient);
	if (!Comp_Interact) return;

	Comp_Interact->RegisterComponent();
	if (!isGetKey) {
		Comp_Interact->SetCantToggle(true);
	}
}

//������
void APuzzleKeyPad::PressEStart()
{
	APuzzleKeyDoor* puzzleDoor = Cast<APuzzleKeyDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleKeyDoor::StaticClass()));
	puzzleDoor->OpenDoor();

	Comp_Interact->DestroyThisComponentFunc();
}

//Ű ����
void APuzzleKeyPad::GainKey()
{
	isGetKey = true;

	if (!Comp_Interact) return;
	Comp_Interact->SetCantToggle(false);
}



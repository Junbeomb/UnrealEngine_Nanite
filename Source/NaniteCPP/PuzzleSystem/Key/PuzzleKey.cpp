
#include "PuzzleKey.h"
#include "../../Blending/Comp_BlendMesh.h"
#include "../../InteractionSystem/Comp_InteractBase.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzleKeyPad.h"

APuzzleKey::APuzzleKey()
{
	KeyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyStaticMesh"));
	RootComponent = KeyStaticMesh;
	
	Comp_Blend = CreateDefaultSubobject<UComp_BlendMesh>(TEXT("Comp_Blend"));
	Comp_Blend->D_FinishBlending.BindUObject(this, &APuzzleKey::OnFinishBlending);
}

//blending이 끝나면 상호작용 활성화
void APuzzleKey::OnFinishBlending()
{
	Comp_Interact =Cast<UComp_InteractBase>(AddComponentByClass(UComp_InteractBase::StaticClass(), false, FTransform::Identity, false));
	if (Comp_Interact) {
		Comp_Interact->RegisterComponent();
	}
}

void APuzzleKey::PressEStart()
{
	APuzzleKeyPad* KeyPad = Cast<APuzzleKeyPad>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleKeyPad::StaticClass()));
	KeyPad->GainKey();
	Destroy();
}


// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaniteCPPCharacter.h"
#include "../BeginnerWeapon/NaniteCPPProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "../InteractionSystem/Comp_InteractBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ANaniteCPPCharacter

ANaniteCPPCharacter::ANaniteCPPCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	HighQualityGun =true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	Mesh1P->SetRelativeLocation(FVector(-20.f, 0.f, -90.f));

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(Mesh1P, "head");
	Mesh1P->SetRelativeRotation(FRotator(-90.0f, 0.0f, 90.0f));
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 10.f, 0.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;


}



void ANaniteCPPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void ANaniteCPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WalkSpeed = GetVelocity().Length();

	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector FinishTrace = FirstPersonCameraComponent->GetForwardVector() * 300.f + StartTrace;
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, FinishTrace, ECollisionChannel::ECC_Visibility)) { //linetrace 성공 시
		if (IsValid(HitResult.GetActor()->GetComponentByClass(UComp_InteractBase::StaticClass()))) { //해당 액터가 UComp_InteractBase를 가지고 있으면
			if (IsValid(LastInteractBase)) { //LastInteractBase안에 값이 있으면
				if (LastInteractBase->GetName() != HitResult.GetActor()->GetName()) { //이름이 같지 않으면
					LastCompBase->TurnOffHover();
				}
			}
			else {
				if (IsValid(LastCompBase)) {
					LastCompBase->TurnOffHover();
					IsInteractBaseHover = false;
				}
			}

			LastInteractBase = HitResult.GetActor();
			LastCompBase = Cast<UComp_InteractBase>(HitResult.GetActor()->GetComponentByClass(UComp_InteractBase::StaticClass()));
			LastCompBase->TurnOnHover();
			IsInteractBaseHover = true;
		}
	}
	else { //lineTrace 안될 시에
		if (IsValid(LastCompBase)) {
			LastCompBase->TurnOffHover();
			IsInteractBaseHover = false;
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ANaniteCPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANaniteCPPCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANaniteCPPCharacter::Look);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ANaniteCPPCharacter::Interact);

		EnhancedInputComponent->BindAction(WatchMapAction, ETriggerEvent::Triggered, this, &ANaniteCPPCharacter::WatchMap);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ANaniteCPPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ANaniteCPPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANaniteCPPCharacter::Interact()
{
	//이부분 이어서 하기
	if (IsInteractBaseHover) {
		LastCompBase->TurnOnToggleFunction();
	}
}

void ANaniteCPPCharacter::WatchMap()
{
	if (AnimWatchMap == 0.f) {
		AnimWatchMap = 1.f;
	}
	else {
		AnimWatchMap = 0.f;
	}
}

void ANaniteCPPCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ANaniteCPPCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ANaniteCPPCharacter::UpScore()
{
	Score++;
}

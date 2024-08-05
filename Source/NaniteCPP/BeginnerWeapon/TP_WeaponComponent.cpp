// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"
#include "NaniteCPPProjectile.h"
#include "../BlackholeProjectile.h"
#include "../Blending/BlendingProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "../BeginnerCharacter/NaniteCPPCharacter.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{

}

void UTP_WeaponComponent::CalculateProjectile()
{
	//UE_LOG(LogTemp, Warning, TEXT("Spawn Actor"));
	World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//라인 트레이스 재료
		FHitResult HitResult;
		FVector StartTrace = PlayerController->PlayerCameraManager->GetCameraLocation();
		FVector FinishTrace = PlayerController->PlayerCameraManager->GetActorForwardVector() * 1000 + StartTrace;

		//디버그
		//DrawDebugLine(GetWorld(), StartTrace, FinishTrace, FColor::Green, true, 1, 0, 1);

		//라인 트레이스
		FVector TargetVector;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, FinishTrace, ECollisionChannel::ECC_Visibility)) {
			TargetVector = HitResult.Location;
		}
		else {
			TargetVector = HitResult.TraceEnd;
		}

		TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetSocketLocation(TEXT("Muzzle")), TargetVector);

		//스폰 셋
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	}
}

void UTP_WeaponComponent::SpawnBlackhole()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	if (BlackholeProjectileClass != nullptr)
	{
		CalculateProjectile();
		World->SpawnActor<ABlackholeProjectile>(BlackholeProjectileClass, GetSocketLocation(TEXT("Muzzle")), TargetRotation, ActorSpawnParams);
	}
}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	if (BlendingProjectileClass != nullptr)
	{
		CalculateProjectile();
		World->SpawnActor<ABlendingProjectile>(BlendingProjectileClass, GetSocketLocation(TEXT("Muzzle")), TargetRotation, ActorSpawnParams);
		
		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}
	
		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
	
}

void UTP_WeaponComponent::AttachWeapon(ANaniteCPPCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (Character == nullptr || Character->GetHasRifle())
	{
		return;
	}
	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("RifleSocket")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{

			//BlendingGun
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			//BlackholeFire
			EnhancedInputComponent->BindAction(FireBlackhole, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::SpawnBlackhole);
		}
	}
}



void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FH_ProjektCharacter.h"
#include "FH_ProjektProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	//MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

}


void UTP_WeaponComponent::Fire(AFH_ProjektCharacter* TargetCharacter,FName sockename)
{
	UWorld* const World = GetWorld();
	if (World != nullptr && Character != nullptr)
	{
		// Hole das Skeletal Mesh des Charakters oder der Waffe (je nachdem, wo der Socket ist)
		USkeletalMeshComponent* MeshComp = Character->GetMesh1P();
		if (MeshComp != nullptr && current_ammo != 0)
		{
			socketNAme = sockename;
			FVector MuzzleLocation = MeshComp->GetSocketLocation(socketNAme);
			FRotator MuzzleRotation = MeshComp->GetSocketRotation(socketNAme);



			// Line Trace und andere Logik
			FCollisionQueryParams queryParams;
			queryParams.AddIgnoredActor(Character);
			FHitResult onHit;
			FVector TraceEnd = MuzzleLocation + (Character->GetControlRotation().Vector() * 15000);

			bool bHit = World->LineTraceSingleByChannel(onHit, MuzzleLocation, TraceEnd, ECollisionChannel::ECC_Pawn, queryParams);
			// DrawDebugLine(World, MuzzleLocation, TraceEnd, FColor::Green, false, 1.0f, 0, 1.0f);

			if (muzzle) {

				FVector SpawnLocation = MuzzleLocation; // Beispiel: Position der Waffe als Spawn-Position
				FRotator SpawnRotation = MuzzleRotation; // Rotation der Waffe wird übernommen
				SpawnRotation.Roll += FMath::RandRange(-45.0f, 45.0f);
				FRotator RandomRotation = FRotator(FMath::RandRange(-180.0f, 180.0f),
					FMath::RandRange(-180.0f, 180.0f),
					FMath::RandRange(-180.0f, 180.0f));

				// Muzzle Flash spawnieren
				flash = GetWorld()->SpawnActor<AMuzzelFlash>(muzzle, SpawnLocation, SpawnRotation);
				FTimerHandle MuzzlePosTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(
					MuzzlePosTimerHandle,             // Timer handle
					this,                             // Object owning the timer
					&UTP_WeaponComponent::SetMuzzlePos, // Method to call
					0.01,                         // Time between calls (in seconds)
					false                             // Loop the timer
				);

				FTimerHandle MuzzlePosTimerHandle6;
				GetWorld()->GetTimerManager().SetTimer(
					MuzzlePosTimerHandle6,             // Timer handle
					this,                             // Object owning the timer
					&UTP_WeaponComponent::SetMuzzlePos, // Method to call
					0.02,                         // Time between calls (in seconds)
					false                             // Loop the timer
				);

				FTimerHandle MuzzlePosTimerHandle3;
				GetWorld()->GetTimerManager().SetTimer(
					MuzzlePosTimerHandle3,             // Timer handle
					this,                             // Object owning the timer
					&UTP_WeaponComponent::SetMuzzlePos, // Method to call
					0.03,                         // Time between calls (in seconds)
					false                             // Loop the timer
				);

				FTimerHandle MMuzzlePosTimerHandle4;
				GetWorld()->GetTimerManager().SetTimer(
					MMuzzlePosTimerHandle4,             // Timer handle
					this,                             // Object owning the timer
					&UTP_WeaponComponent::SetMuzzlePos, // Method to call
					0.04,                         // Time between calls (in seconds)
					false                             // Loop the timer
				);

				FTimerHandle MMuzzlePosTimerHandle5;
				GetWorld()->GetTimerManager().SetTimer(
					MMuzzlePosTimerHandle5,             // Timer handle
					this,                             // Object owning the timer
					&UTP_WeaponComponent::SetMuzzlePos, // Method to call
					0.05,                         // Time between calls (in seconds)
					false                             // Loop the timer
				);

				FTimerHandle MMuzzlePosTimerHandle2;
				GetWorld()->GetTimerManager().SetTimer(
					MMuzzlePosTimerHandle2,             // Timer handle
					this,                             // Object owning the timer
					&UTP_WeaponComponent::SetMuzzlePos, // Method to call
					0.06,                         // Time between calls (in seconds)
					false                             // Loop the timer
				);

			}

			/*if (NiagaraComponent)
			{
				NiagaraComponent->SetWorldRotation(MuzzleRotation);
				//NiagaraComponent->SetWorldLocation(MuzzleOffset);
				//NiagaraComponent->SetWorldLocation(MuzzleLocation);
				NiagaraComponent->Activate();
			}*/

			GetWorld()->GetTimerManager().SetTimer(
				DeactivationTimerHandle, // Timer-Handle
				this,                    // Zielobjekt
				&UTP_WeaponComponent::DeactivateNiagaraEffect, // Methode, die aufgerufen wird
				0.1f,                    // Verzögerung in Sekunden
				false                    // Kein Looping
			);

			lostAmmo();

			if (bHit && onHit.GetActor() != nullptr)
			{
				AActor* HitActor = onHit.GetActor();
				AEnemy* HitEnemy = Cast<AEnemy>(HitActor);

				if (HitEnemy)
				{
					if (HitEnemy->ActorHasTag(FName("Enemy")))
					{
						UE_LOG(LogTemp, Log, TEXT("Hit enemy at location: %s"), *onHit.ImpactPoint.ToString());
						bool getPoint = HitEnemy->GetDmgEnemy(dmg, onHit.ImpactPoint, onHit.ImpactNormal);
						if (getPoint)
						{
							TargetCharacter->killscore += 1;
							TargetCharacter->MyGameInstance->killscore += 1;
						}
					}
				}
			}
		}

		// Abspielen des Feuer-Sounds (falls vorhanden)
		if (fire_sound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, fire_sound, Character->GetActorLocation());
		}

		if (empty_sound != nullptr && current_ammo == 0)
		{
			UGameplayStatics::PlaySoundAtLocation(this, empty_sound, Character->GetActorLocation());
		}
	}
}

void UTP_WeaponComponent::SetMuzzlePos()
{
	USkeletalMeshComponent* MeshComp = Character->GetMesh1P();
	flash->SetActorLocation(MeshComp->GetSocketLocation(socketNAme));
	UE_LOG(LogTemp, Warning, TEXT("WeaponComponent is null!"));
}

void UTP_WeaponComponent::lostAmmo()
{
	current_ammo -= 1;
}
void UTP_WeaponComponent::GetAmmo_R()
{
	if (Character && reload_sound) {
		UGameplayStatics::PlaySoundAtLocation(this, reload_sound, Character->GetActorLocation());
		current_ammo = max_ammo;

	}
	
}
void UTP_WeaponComponent::ApplyKnockbackAndStun(AEnemy* HitEnemy)
{
	UWorld* const World = GetWorld();
	// 1. Bewegung stoppen
	UCharacterMovementComponent* MovementComponent = HitEnemy->FindComponentByClass<UCharacterMovementComponent>();
	speed_enemy = HitEnemy->speed;
	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = 0;  // Geschwindigkeit auf 0 setzen

		// Timer für 0,2 Sekunden, danach wird die Geschwindigkeit wiederhergestellt
		FTimerHandle UnusedHandle;
		World->GetTimerManager().SetTimer(UnusedHandle, [MovementComponent]()
			{
				MovementComponent->MaxWalkSpeed = 300.0f;  // Ursprüngliche Geschwindigkeit wiederherstellen (anpassen falls nötig)
			}, 0.2f, false);

	}

	// 2. Rückstoß anwenden
	FVector KnockbackDirection = -HitEnemy->GetActorForwardVector();  // Entgegengesetzte Richtung
	FVector KnockbackForce = KnockbackDirection * 500.0f;  // Passe die Stärke an (500.0f ist Beispielwert)
	HitEnemy->LaunchCharacter(KnockbackForce, true, true);  // Rückstoß anwenden
}

void UTP_WeaponComponent::DeactivateNiagaraEffect()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
}

FVector UTP_WeaponComponent::GetSocketPos()
{
	USkeletalMeshComponent* MeshComp = Character->GetMesh1P();
	FName MuzzleSocketName;
	FVector MuzzleLocation;
	if (MeshComp) {
		MuzzleSocketName = TEXT("shotLoc");
	    MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	}
	return MuzzleLocation;
}



bool UTP_WeaponComponent::AttachWeapon(AFH_ProjektCharacter* TargetCharacter, FName socket)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr)
	{
		return false;
	}

	if (NiagaraComponent)
	{
		NiagaraComponent->AttachToComponent(
			Character->GetMesh1P(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("shotLoc")
		);

	}
	// Attach the weapon to the First Person Character


	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(socket));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

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
			// Fire

		}
	}

	return true;
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
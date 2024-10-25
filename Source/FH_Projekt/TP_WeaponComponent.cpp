
// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FH_ProjektCharacter.h"
#include "FH_ProjektProjectile.h"
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

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	clipsize = current_ammo;
}


void UTP_WeaponComponent::Fire(AFH_ProjektCharacter* TargetCharacter)
{
	// Sicherstellen, dass die Welt vorhanden ist
	UWorld* const World = GetWorld();
	if (World != nullptr && Character != nullptr)
	{
		// Hole das Skeletal Mesh des Charakters oder der Waffe (je nachdem, wo der Socket ist)
		USkeletalMeshComponent* MeshComp = Character->GetMesh1P(); // Oder verwende das Waffenskeletalmesh, wenn die Waffe ein separates Mesh hat
		if (MeshComp != nullptr && current_ammo != 0)
		{
			// Definiere den Namen des Sockets (z. B. "Muzzle" für die Waffe)
			FName MuzzleSocketName = TEXT("shotLoc");
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			// Hole die Kamerarotation
			FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

			// Reduziere den Pitch (Y-Achse), um die Rotation nach unten zu verschieben
			/*float PitchOffset = -5.0f; // Negative Werte neigen nach unten, positive nach oben
			FRotator AdjustedRotation = CameraRotation;
			AdjustedRotation.Pitch += PitchOffset;
			const FRotator SpawnRotation = AdjustedRotation;*/

			// Hole die Position und Rotation des Sockets
			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);
			
			// Optional: Ignoriere den eigenen Actor beim Trace, um keine Kollision mit sich selbst zu verursachen
			FCollisionQueryParams queryParams;
			queryParams.AddIgnoredActor(Character);

			// Führe den Line Trace vom Muzzle-Socket durch
			FHitResult onHit;
			FVector TraceEnd = MuzzleLocation + (CameraRotation.Vector() * 3000); // 3000 Einheiten nach vorne

			bool bHit = World->LineTraceSingleByChannel(onHit, MuzzleLocation, TraceEnd, ECollisionChannel::ECC_Pawn, queryParams);
			lostAmmo();

			// Debug Line zeichnen, um den Trace zu visualisieren
			DrawDebugLine(World, MuzzleLocation, TraceEnd, FColor::Red, false, 5.f, 0, 5.f);

			if (bHit && onHit.GetActor() != nullptr)
			{
				AActor* HitActor = onHit.GetActor();  // Der getroffene Actor

				// Versuche, den Actor auf AEnemy zu casten
				AEnemy* HitEnemy = Cast<AEnemy>(HitActor);

				if (HitEnemy)
				{
					// Erfolgreiches Casting: Der getroffene Actor ist ein AEnemy
					if (HitEnemy->ActorHasTag(FName("Enemy")))  // Überprüfe den Tag
					{
						UE_LOG(LogTemp, Log, TEXT("Line Trace hit enemy with tag: %s"), *HitEnemy->GetName());
						HitEnemy->GetDmgEnemy(100);
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("Line Trace hit enemy, but it does not have the tag: %s"), *HitEnemy->GetName());
					}
				}
				else
				{
					// Das Casting ist fehlgeschlagen, der Actor ist kein AEnemy
					UE_LOG(LogTemp, Log, TEXT("Line Trace hit actor, but it's not an AEnemy: %s"), *HitActor->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Line Trace did not hit anything."));
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

		// Hier könntest du auch noch Animationen für den Schuss hinzufügen, falls erforderlich
	}
}
void UTP_WeaponComponent::lostAmmo()
{
	current_ammo -= 1;
}
void UTP_WeaponComponent::GetAmmo_R()
{
	UGameplayStatics::PlaySoundAtLocation(this, reload_sound, Character->GetActorLocation());
	current_ammo = clipsize;
}
bool UTP_WeaponComponent::AttachWeapon(AFH_ProjektCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("weaponsocket")));

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
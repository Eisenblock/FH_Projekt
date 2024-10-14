// Copyright Epic Games, Inc. All Rights Reserved.

#include "FH_ProjektCharacter.h"
#include "FH_ProjektProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "TP_WeaponComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFH_ProjektCharacter

AFH_ProjektCharacter::AFH_ProjektCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AFH_ProjektCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	CurrentWeaponComponent = EquipWeapon();
}

//////////////////////////////////////////////////////////////////////////// Input

void AFH_ProjektCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AFH_ProjektCharacter::Attack);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFH_ProjektCharacter::Reload);

	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFH_ProjektCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFH_ProjektCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AFH_ProjektCharacter::Attack()
{
	UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();

	// Überprüfen, ob der AnimInstance existiert
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist nullptr!"));
		return;
	}

	// Überprüfen, ob die Animationsmontage zugewiesen ist
	if (shoot_anim)
	{
		AnimInstance->Montage_Play(shoot_anim, 1.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("shoot_anim ist nullptr!"));
	}

	// Unabhängig von der Animation feuert die Waffe
	if (CurrentWeaponComponent)
	{
		CurrentWeaponComponent->Fire(this);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Kein CurrentWeaponComponent zum Feuern."));
	}
}

void AFH_ProjektCharacter::Reload()
{
	UAnimInstance* animInstance = GetMesh1P()->GetAnimInstance();
	if (animInstance != nullptr)
	{
		animInstance->Montage_Play(reload_anim, 1.0f);
	}
}

UTP_WeaponComponent* AFH_ProjektCharacter::EquipWeapon()
{
	// Hole den PlayerController und überprüfe, ob er gültig ist
	APlayerController* pControll = Cast<APlayerController>(GetController());
	if (!pControll)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is invalid"));
		return nullptr;  // Kein gültiger Controller, frühzeitig abbrechen
	}

	// Hole die Kamerarotation und Position des Charakters
	const FRotator pRota = pControll->PlayerCameraManager->GetCameraRotation();
	const FVector pLoc = GetMesh1P()->GetSocketLocation(FName("weaponsocket"));  // Verwende GetActorLocation(), nicht GetOwner()

	// Spawnparameter
	FActorSpawnParameters pSpawnParam;
	pSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawne die Waffe (p_weapon muss korrekt als TSubclassOf<AActor> deklariert sein)
	AActor* pWeapon = GetWorld()->SpawnActor<AActor>(p_weapon, pLoc, pRota, pSpawnParam);
	if (!pWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn weapon"));
		return nullptr;
	}

	// Hole die Waffenkomponente und hänge die Waffe an den Charakter an
	UTP_WeaponComponent* pWeeapon = pWeapon->FindComponentByClass<UTP_WeaponComponent>();
	if (pWeeapon)
	{
		pWeeapon->AttachWeapon(this);  // Hier wird die Waffe an den Charakter angehängt
		CurrentWeaponComponent = pWeeapon; // Speichere die Waffenkomponente in der Charakterklasse
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon component not found on spawned weapon"));
		return nullptr;
	}

	return pWeeapon;  // Rückgabe der Waffenkomponente
}

void AFH_ProjektCharacter::Move(const FInputActionValue& Value)
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

void AFH_ProjektCharacter::Look(const FInputActionValue& Value)
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
// Copyright Epic Games, Inc. All Rights Reserved.

#include "FH_ProjektCharacter.h"
#include "EnemySpawn.h"
#include "FH_ProjektProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "MyGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_WeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
	Tags.Add(FName("Player"));
	
}

void AFH_ProjektCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	CurrentLevelName = GetWorld()->GetMapName();
	//CurrentLevelName = FPaths::GetBaseFilename(CurrentLevelName);
	MovementComponent = GetCharacterMovement();
	current_weapon = s_weapon;
	UE_LOG(LogTemp, Warning, TEXT("Current Level Name: %s"), *CurrentLevelName);
	if (CurrentLevelName == TEXT("UEDPIE_0_ArenaTest") || CurrentLevelName == TEXT("UEDPIE_0_TestMap"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Level Name: %s"), *CurrentLevelName);
		m_WeaponComponent = EquipWeapon(p_weapon, FName("weaponsocket_1"));
		s_WeaponComponent = EquipWeapon(s_weapon, FName("weaponsocket"));	
		m_weaponsA->SetActorHiddenInGame(true);
	}
	

	if (GetWorld()->GetMapName() == TEXT("UEDPIE_0_ArenaTest1") || CurrentLevelName == TEXT("UEDPIE_0_TestMap1"))
	{
		s_WeaponComponent = EquipWeapon(s_weapon, FName("weaponsocket"));
		m_WeaponComponent = EquipWeapon(p_weapon, FName("weaponsocket_1"));
		s_weaponsA->SetActorHiddenInGame(true);
	}
	//m_WeaponComponent = EquipWeapon(p_weapon, FName("weaponsocket_1"));
    //s_WeaponComponent = EquipWeapon(s_weapon,FName("weaponsocket"));
	
	
	
	



	
	
	if (m_cPlayerHUD != nullptr)
	{
		//add the HUd to the viewport
		UUserWidget* HUD = CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), m_cPlayerHUD);
		HUD->AddToViewport(9999);

	}

	if (gameOverScreen != nullptr)
	{
		//add the HUd to the viewport
		UUserWidget* HUD = CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), gameOverScreen);
		HUD->AddToViewport(9999);

	}
	UWorld* World = GetWorld();
	if (World)
	{
		MyGameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
		if (MyGameInstance)
		{
			life = MyGameInstance->PlayerLife;  // Lebenswert wiederherstellen
			killscore = MyGameInstance->killscore;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MyGameInstance is not valid!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() is invalid!"));
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AFH_ProjektCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	


	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AFH_ProjektCharacter::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AFH_ProjektCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AFH_ProjektCharacter::StopAttack);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFH_ProjektCharacter::Reload);
	//PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AFH_ProjektCharacter::ChangeWeapon);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFH_ProjektCharacter::DoSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFH_ProjektCharacter::GetNormalSpeed);
	
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

void AFH_ProjektCharacter::Tick(float DeltaTime)
{
	current_ammo = CurrentWeaponComponent->current_ammo;
	max_ammo = CurrentWeaponComponent->max_ammo;

	

	timer += DeltaTime;
	
	if (changeMap_timer <= 0)
	{
		textChangeMap = true;
	}

	if (lvlStart && !textChangeMap)
	{
		changeMap_timer -= DeltaTime;
	}

	
	if (current_ammo == 0 && !breload)
	{
		Reload();
		breload = true;

		GetWorld()->GetTimerManager().SetTimer(
			reloadTimerHandle,     // Timer handle
			this,                       // Target object
			&AFH_ProjektCharacter::Resetreload, // Method to call
			3.0f,                       // Time in seconds
			false                       // Do not loop
		);
	}

	/*float a = 4;
	if ( a <= killscore) 
	{
		MyGameInstance->Changemap();
	}*/
}



void AFH_ProjektCharacter::ReloadMap()
{
	if (UWorld* World = GetWorld())
	{


		FString CurrentLevelNamee = World->GetMapName();
		CurrentLevelNamee.RemoveFromStart(World->StreamingLevelsPrefix);


		TArray<AActor*> ActorsWithTag;
		FName DesiredTag = FName("Spawn");
		UGameplayStatics::GetAllActorsWithTag(World, DesiredTag, ActorsWithTag);


		AEnemySpawn* EnemySpawnActor = nullptr;
		if (ActorsWithTag.Num() > 0)
		{
			EnemySpawnActor = Cast<AEnemySpawn>(ActorsWithTag[0]);
			if (EnemySpawnActor)
			{
				UE_LOG(LogTemp, Log, TEXT("Found AEnemySpawn actor with tag '%s'"), *DesiredTag.ToString());
				EnemySpawnActor->ClearTimer();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor with tag '%s' is not of type AEnemySpawn"), *DesiredTag.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No actors found with tag '%s'"), *DesiredTag.ToString());
		}


		UGameplayStatics::OpenLevel(World, TEXT("ArenaTest"));
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
	if (shoot_anim && CurrentWeaponComponent->current_ammo != 0 && CurrentWeaponComponent == s_WeaponComponent )
	{
		AnimInstance->Montage_Play(shoot_anim, 1.0f);
		FName MuzzleSocketName = TEXT("shotLoc");
		
		//FVector MuzzleLocation = GetMesh1PGetSocketLocation(MuzzleSocketName);
		CurrentWeaponComponent->Fire(this, MuzzleSocketName);
		
	}
	
	if (shoot_anim && CurrentWeaponComponent->current_ammo != 0 && CurrentWeaponComponent == m_WeaponComponent) {
		AnimInstance->Montage_Play(shoot_anim, 1.0f);
		FName MuzzleSocketName = TEXT("shotLoc1");

		//FVector MuzzleLocation = GetMesh1PGetSocketLocation(MuzzleSocketName);
		CurrentWeaponComponent->Fire(this,MuzzleSocketName);
	}

	// Unabhängig von der Animation feuert die Waffe
	/*if (CurrentWeaponComponent)
	{
		CurrentWeaponComponent->Fire(this);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Kein CurrentWeaponComponent zum Feuern."));
	}*/
}

void AFH_ProjektCharacter::StartAttack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true; // Setze den Angriffsstatus
		Attack(); // Führe sofort den ersten Angriff aus

		// Starte den Timer, der alle 0.1 Sekunden die Attack-Funktion aufruft
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AFH_ProjektCharacter::Attack, CurrentWeaponComponent->attackSpeed, true);
	}

}

void AFH_ProjektCharacter::StopAttack()
{
	bIsAttacking = false; // Setze den Angriffsstatus zurück
    GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void AFH_ProjektCharacter::Reload()
{
	UAnimInstance* animInstance = GetMesh1P()->GetAnimInstance();
	if (animInstance != nullptr )
	{
		animInstance->Montage_Play(reload_anim, 1.0f);
		//CurrentWeaponComponent->GetAmmo_R();
	}
}

void AFH_ProjektCharacter::GetDmg(float dmg)
{
	feedback_dmg = true;
	life -= dmg;
	GetWorld()->GetTimerManager().SetTimer(
		FeedbackDmgTimerHandle,     // Timer handle
		this,                       // Target object
		&AFH_ProjektCharacter::ResetFeedbackDmg, // Method to call
		2.0f,                       // Time in seconds
		false                       // Do not loop
	);
	MyGameInstance->PlayerLife = life;
	
	UE_LOG(LogTemp, Warning, TEXT("Life after damage: %f"), life);
	if (life <= 0) 
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (PlayerController)
		{
			// Setze den Input Mode auf Game and UI, um die Maus zu sehen und zu interagieren
			FInputModeGameAndUI InputMode;
			PlayerController->SetInputMode(InputMode);

			// Mache die Maus sichtbar
			PlayerController->bShowMouseCursor = true;
		}
		playerDead = true;
		MyGameInstance->PlayerLife = 100.0f;
	}
}

UTP_WeaponComponent* AFH_ProjektCharacter::EquipWeapon(TSubclassOf<class AActor> weapon,FName socketname)
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
	const FVector pLoc = GetMesh1P()->GetSocketLocation(FName(socketname));  // Verwende GetActorLocation(), nicht GetOwner()

	// Spawnparameter
	FActorSpawnParameters pSpawnParam;
	pSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawne die Waffe (p_weapon muss korrekt als TSubclassOf<AActor> deklariert sein)
	UTP_WeaponComponent* pWeeapon;
	if (weapon == s_weapon)
	{
		s_weaponsA = GetWorld()->SpawnActor<AActor>(weapon, pLoc, pRota, pSpawnParam);
		pWeeapon = s_weaponsA->FindComponentByClass<UTP_WeaponComponent>();
	}
	else
	{
		m_weaponsA = GetWorld()->SpawnActor<AActor>(weapon, pLoc, pRota, pSpawnParam);
		pWeeapon = m_weaponsA->FindComponentByClass<UTP_WeaponComponent>();
	}

	if (pWeeapon)
	{
		pWeeapon->AttachWeapon(this,socketname);  // Hier wird die Waffe an den Charakter angehängt
		CurrentWeaponComponent = pWeeapon; // Speichere die Waffenkomponente in der Charakterklasse
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon component not found on spawned weapon"));
		return nullptr;
	}

	return pWeeapon;  // Rückgabe der Waffenkomponente
}

void AFH_ProjektCharacter::ChangeWeapon()
{
	UAnimInstance* animInstance = GetMesh1P()->GetAnimInstance();
	if (current_weapon == s_weapon)
	{
		//animInstance->Montage_Play(reload_anim,1.0f);
		s_weaponsA->SetActorHiddenInGame(true);
		m_weaponsA->SetActorHiddenInGame(false);
		current_weapon = p_weapon;
		CurrentWeaponComponent = m_WeaponComponent;
	}
	else
	{
		//animInstance->Montage_Play(reload_anim,1.0f);
		m_weaponsA->SetActorHiddenInGame(true);		
		s_weaponsA->SetActorHiddenInGame(false);
		current_weapon = s_weapon;
		CurrentWeaponComponent = s_WeaponComponent;
	}
}

void AFH_ProjektCharacter::GetLife(float life_amount)
{
	life += life_amount;
	if (life > 100)
	{
		life = 100.0f;
	}
}

void AFH_ProjektCharacter::ApplyImpulse(const FVector& Direction, float ImpulseStrength)
{
	if (GetCharacterMovement())
	{
		// Anwendung des Impulses in die angegebene Richtung
		LaunchCharacter(Direction * ImpulseStrength, true, true);  // bool Wert 1: Ignore current velocity, bool Wert 2: Affects gravity
	}
}

void AFH_ProjektCharacter::ResetFeedbackDmg()
{
	feedback_dmg = false;
}

void AFH_ProjektCharacter::Resetreload()
{
	breload = false;
}

void AFH_ProjektCharacter::DoSprint()
{
	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = 1200.0f;
		isWalking = true;
	}
}

void AFH_ProjektCharacter::GetNormalSpeed()
{
	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = 600.0f;
		isWalking = false;
	}
}

void AFH_ProjektCharacter::ActivateLvl()
{
	lvlStart = true;
}

bool AFH_ProjektCharacter::IsWalking() const
{
	return isWalking;
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
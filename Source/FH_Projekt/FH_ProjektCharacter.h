// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MyGameInstance.h"
#include "TP_WeaponComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/SpotLightComponent.h"
#include "FH_ProjektCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFH_ProjektCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	AFH_ProjektCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* reload_anim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* shoot_anim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AActor> current_weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AActor> p_weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AActor> s_weapon;

	AActor* s_weaponsA;
	AActor* m_weaponsA;

	//reference to HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> m_cPlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> gameOverScreen;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> WinScreen;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool player_win = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> EscScreen;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool escB = false;


	//Weapon Stats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool weaponType_Pistol = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool weaponType_MG = false;
	FTimerHandle AttackTimerHandle;
	bool bIsAttacking = false;
	bool breload = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool lvlStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool inLab = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool activePortal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool playerDead = false;
	FString CurrentLevelName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool isWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool feedback_dmg = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	int32 current_ammo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	int32 max_ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	USpotLightComponent* WeaponLight;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	float life;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	float killscore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float timer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float changeMap_timer = 120.0f;

	//AUdio

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	bool firstMap;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* SoundCue;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* SoundCue2;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UAudioComponent* AudioComponent;
	
	UTP_WeaponComponent* CurrentWeaponComponent;
	UTP_WeaponComponent* m_WeaponComponent;
	UTP_WeaponComponent* s_WeaponComponent;
	UCharacterMovementComponent* MovementComponent;
	UMyGameInstance* MyGameInstance;
	FTimerHandle  FeedbackDmgTimerHandle;
	FTimerHandle  reloadTimerHandle;

	//Bool CHange Map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	bool textChangeMap = false;
	UFUNCTION(BlueprintCallable)
	void ReloadMap();

	

	
	void Attack();
	void StartAttack();
	void StopAttack();
	void Reload();
	void GetDmg(float dmg);
	UTP_WeaponComponent* EquipWeapon(TSubclassOf<class AActor> weapon,FName socketname);
	void ChangeWeapon();
	void GetLife(float life_amount);
	void ApplyImpulse(const FVector& Direction, float ImpulseStrength);
	// Reset feedback_dmg
	void ResetFeedbackDmg();
	void Resetreload();
	void DoSprint();
	void GetNormalSpeed();
	void ActivateLvl();
	void SetPortalTrue();
	void ResetFindPortal();
	void PlayerWInGame();
	void ActivateESCScreen();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void DeActivateESCScreen();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ChangeMap(FName mapName);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsWalking() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SwitchToMainMenu();




protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	virtual void Tick(float DeltaTime) override;
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};


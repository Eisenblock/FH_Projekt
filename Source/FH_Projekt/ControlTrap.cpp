// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlTrap.h"
#include "DamageZone.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AControlTrap::AControlTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AControlTrap::BeginPlay()
{
    Super::BeginPlay();

    // Find all actors with the "Trap" tag
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), trapName, Traps);
    DeaktivateAllTraps();

    if (Traps.Num() > 0)
    {
        // Start the timer to activate traps in sequence
        GetWorld()->GetTimerManager().SetTimer(
            TrapActivationTimerHandle,
            this,
            &AControlTrap::ActivateNextTrap,
            trapIntervall,
            true
        );
    }
}

// Called every frame
void AControlTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AControlTrap::ActivateNextTrap()
{
    if (Traps.Num() == 0) return;

    // Deactivate the current trap
    if (CurrentTrapIndex >= 0 && CurrentTrapIndex < Traps.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Deactivating trap"));

        if (oldValue >= 0 && oldValue < Traps.Num())
        {
            AActor* PreviousTrap = Traps[oldValue];
            ADamageZone* PreviousTrapZone = Cast<ADamageZone>(PreviousTrap);
            if (PreviousTrapZone)
            {
               // PreviousTrapZone->CleanColllision();
               PreviousTrapZone->SetActorHiddenInGame(true);
                PreviousTrapZone->SetActorEnableCollision(false);
                PreviousTrapZone->SetActorTickEnabled(false);
                PreviousTrapZone->NiagaraComponent->Deactivate();
                UE_LOG(LogTemp, Warning, TEXT("Trap %s deactivated"), *PreviousTrap->GetName());
            }
        }

        // Activate the current trap
        AActor* CurrentTrap = Traps[CurrentTrapIndex];
        if (CurrentTrap)
        {
            ADamageZone* CurrentTrapZone = Cast<ADamageZone>(CurrentTrap);
            if (CurrentTrapZone)
            {
                CurrentTrapZone->ActivateTrap();
               /*CurrentTrapZone->SetActorHiddenInGame(false);
                CurrentTrapZone->SetActorEnableCollision(true);
                CurrentTrapZone->SetActorTickEnabled(true);*/ 
                UE_LOG(LogTemp, Warning, TEXT("Trap %s activated"), *CurrentTrap->GetName());
            }
        }

        // Update indices
        oldValue = CurrentTrapIndex;
        CurrentTrapIndex += 1;

        if (CurrentTrapIndex >= Traps.Num())
        {
            CurrentTrapIndex = 0;
        }
    }

}

void AControlTrap::DeaktivateAllTraps()
{

    int a = 0;
    for (a = 0; a < Traps.Num(); a++) 
    {
        AActor* trapA = Traps[a];
        ADamageZone* trap = Cast<ADamageZone>(trapA);
        trap->SetActorHiddenInGame(true); // Versteckt den Actor
        trap->SetActorEnableCollision(false);
        trap->SetActorTickEnabled(false);
    }
}


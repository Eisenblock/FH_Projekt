// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"
#include "Engine/World.h"
#include "Enemy.h"

#include "FH_ProjektCharacter.h"
#include "DamageZone.h"
#include "Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "FH_ProjektCharacter.h"
#include "MyGameInstance.h"
#include "Components/BoxComponent.h"
#include "Ball_AIController.h"
#include "Portal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Konstruktor: Standardwerte setzen
AEnemySpawn::AEnemySpawn()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxEnemies = 10; 
    CurrentEnemies = 0; 
    
    colliderSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderSpawn"));

    // Set it as the RootComponent if needed
    RootComponent = colliderSpawn;

    // Configure collision properties
    colliderSpawn->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    colliderSpawn->SetCollisionObjectType(ECC_WorldDynamic);
    colliderSpawn->SetCollisionResponseToAllChannels(ECR_Overlap);
    colliderSpawn->SetGenerateOverlapEvents(true);

    // Bind the overlap event
    //colliderSpawn->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawn::OnOverlapBegin);
  
}

// Wird aufgerufen, wenn das Spiel beginnt oder der Actor gespawnt wird
void AEnemySpawn::BeginPlay()
{
    Super::BeginPlay();

    //this->SetActorEnableCollision(true);

    TArray<AActor*> allPlayer;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName("Player"),allPlayer);
    characterPlayer = Cast<AFH_ProjektCharacter>( allPlayer[0]);
    SetDamageArea();
    ColletcWayPoints();
    // Spawning wird bei Spielstart gestartet
    //GetWorldTimerManager().SetTimer(UpdateIndexTimerHandle, this, &AEnemySpawn::StartCountWaypoint, 15.0f, true);
    //StartSpawning(Enemy_SlowAF);
    //StartEnemyGetLife(EnemyClass);

    UWorld* World = GetWorld();
    if (World)
    {
        MyGameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
        CurrentLevelName = GetWorld()->GetMapName();
        CurrentLevelName = FPaths::GetBaseFilename(CurrentLevelName);
        if (MyGameInstance)
        {

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



// Spawning-Logik
void AEnemySpawn::StartSpawning(AEnemy* EnemyInstance)
{

    // Timer, der die SpawnEnemy-Funktion regelm��ig aufruft
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, [this]()
        {

            if (WaypointsArray_North.Num() > 0 && b == 0) // Sicherstellen, dass das Array Elemente enth�lt
            {
                int32 a = FMath::RandRange(0, WaypointsArray_North.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_North[a];
                SpawnEnemy(SpawnLocation); // Rufe die SpawnEnemy-Funktion mit dem Standort auf
                UE_LOG(LogTemp, Warning, TEXT("North"));
                b = 1;
            }
            if (WaypointsArray_East.Num() > 0 && b == 1)
            {
                int32 a = FMath::RandRange(0, WaypointsArray_East.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_East[a];
                SpawnEnemy(SpawnLocation);
                UE_LOG(LogTemp, Warning, TEXT("East"));
                b = 2;
            }
            if (WaypointsArray_South.Num() > 0 && b == 2) // Sicherstellen, dass das Array Elemente enth�lt
            {
                int32 a = FMath::RandRange(0, WaypointsArray_South.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_South[a];
                SpawnEnemy(SpawnLocation); // Rufe die SpawnEnemy-Funktion mit dem Standort auf
                UE_LOG(LogTemp, Warning, TEXT("South"));
                b = 3;
            }
            if (WaypointsArray_West.Num() > 0 && b == 3)
            {
                int32 a = FMath::RandRange(0, WaypointsArray_West.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_West[a];
                SpawnEnemy(SpawnLocation);
                UE_LOG(LogTemp, Warning, TEXT("West"));
                b = 0;
            }
        }, SpawnInterval, true);

}

void AEnemySpawn::StartEnemyGetLife(TSubclassOf<AEnemy> Enemy)
{

}

void AEnemySpawn::EnemyGetLife(TSubclassOf<AEnemy> Enemy)
{
    UE_LOG(LogTemp, Log, TEXT("Enemy versucht mehr Leben"));
    extralife += 100.0f;

    /*if (Enemy)
    {
        // Zugriff auf das Default-Objekt der Klasse AEnemy
        AEnemy* DefaultEnemy = Enemy->GetDefaultObject<AEnemy>();
        if (DefaultEnemy)
        {
            // Das Standardleben (z. B. Health) �ndern
            DefaultEnemy->life += 100.0f;  // Angenommen, Health ist eine �ffentliche Variable oder hat eine Setter-Methode
            UE_LOG(LogTemp, Log, TEXT("Neues Standardleben f�r AEnemy-Klasse gesetzt: %f"), DefaultEnemy->life);
        }
    }*/
}

void AEnemySpawn::ClearTimer()
{
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    GetWorld()->GetTimerManager().ClearTimer(ActivationTimer);
    GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
    bTriggeredClearTimers = true;
}

// SpawnEnemy: Spawnt einen Gegner an der Position des Spawners
void AEnemySpawn::SpawnEnemy(AActor* posSpawn)
{

    if (CurrentLevelName != "Testmap" && !bTriggeredClearTimers )
    {
        if (EnemyClasses.Num() > 0) // Ensure there are enemy classes set
        {
            // Randomly select an enemy class from the array
            //int32 RandomIndex = FMath::RandRange(1, 10);
          
            TSubclassOf<AEnemy> SelectedEnemyClass = EnemyClasses[0];
            UE_LOG(LogTemp, Warning, TEXT("Initial controllloop value=%d"), controllloop);

            if (controllloop == 0)
            { 
                SelectedEnemyClass = EnemyClasses[0];
             
            }

            if (controllloop == 1)
            {
                SelectedEnemyClass = EnemyClasses[0];
                
            }

            if (controllloop == 2)
            {
                SelectedEnemyClass = EnemyClasses[2];
                
            }

            if (controllloop == 3)
            {
                SelectedEnemyClass = EnemyClasses[1];
                
            }

            if (controllloop == 4)
            {
                SelectedEnemyClass = EnemyClasses[0];
                
            }

            if (controllloop == 5)
            {
                SelectedEnemyClass = EnemyClasses[3];
                
            }

            if (controllloop == 6)
            {
                SelectedEnemyClass = EnemyClasses[0];
                
            }

            if (controllloop == 7)
            {
                SelectedEnemyClass = EnemyClasses[2];
                
            }

            if (controllloop == 8)
            {
                SelectedEnemyClass = EnemyClasses[0];

            }
           



            //Old Loop
            /*
            if (RandomIndex <= 1)
            {
                SelectedEnemyClass = EnemyClasses[0];
            }
            
            if(RandomIndex <= 3 && RandomIndex >= 2)
            {
                SelectedEnemyClass = EnemyClasses[1];
            }

            if (RandomIndex <= 5 && RandomIndex >= 4)
            {
                SelectedEnemyClass = EnemyClasses[2];
            }

            if (RandomIndex <= 10 && RandomIndex >= 6)
            {
                SelectedEnemyClass = EnemyClasses[3];
            }*/



            if (SelectedEnemyClass && !bTriggeredClearTimers)
            {
                FVector SpawnLocation = posSpawn->GetActorLocation();
                FRotator SpawnRotation = posSpawn->GetActorRotation();

                // Spawn the enemy
                if (!bTriggeredClearTimers) { 
                    Enemy_SlowAF = GetWorld()->SpawnActor<AEnemy>(SelectedEnemyClass, SpawnLocation, SpawnRotation);
                }
             
                spawnCount += 1;
                controllloop += 1;
                UE_LOG(LogTemp, Warning, TEXT("Initial controllloop value=%d"), controllloop);

                if (Enemy_SlowAF)
                {
                    // Set the AIController for the enemy
                    ABall_AIController* AIController = Cast<ABall_AIController>(Enemy_SlowAF->GetController());

                    if (!AIController && !bTriggeredClearTimers)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("AIController was not automatically assigned, assigning manually."));
                        AIController = GetWorld()->SpawnActor<ABall_AIController>(ABall_AIController::StaticClass(), SpawnLocation, SpawnRotation);
                        if (AIController)
                        {
                            AIController->Possess(Enemy_SlowAF); // AIController takes control of the enemy
                        }
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to spawn Enemy!"));
                }

                if (controllloop == 9)
                {
                    controllloop = 0;
                }

            }
           
        }
        
    }
   
}

void AEnemySpawn::ColletcWayPoints()
{
    TArray<AActor*> WaypointActors_South;
    TArray<AActor*> WaypointActors_North;
    TArray<AActor*> WaypointActors_West;
    TArray<AActor*> WaypointActors_East;


    // Suche alle Actor-Instanzen mit dem Tag "waypoint"
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_North"), WaypointActors_North);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden North: %d"), WaypointActors_North.Num());
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_East"), WaypointActors_East);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden East: %d"), WaypointActors_East.Num());
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_South"), WaypointActors_South);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden South: %d"), WaypointActors_South.Num());
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_West"), WaypointActors_West);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden West: %d"), WaypointActors_West.Num());



    /*// �berpr�fe, ob Wegpunkte gefunden wurden
    if (WaypointActors.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Keine Wegpunkte mit dem Tag 'waypoint' gefunden."));
        return;
    }*/

    // Debug-Ausgabe: Anzahl der gefundenen Wegpunkte
    //UE_LOG(LogTemp, Log, TEXT("Anzahl gefundener Wegpunkte: %d"), WaypointActors.Num());

    // Speichere die Wegpunkte im Character-Array, wenn du ein Array f�r sie hast
    this->WaypointsArray_North = WaypointActors_North;
    this->WaypointsArray_East = WaypointActors_East;
    this->WaypointsArray_South = WaypointActors_South;
    this->WaypointsArray_West = WaypointActors_West;

}

void AEnemySpawn::StartCountWaypoint()
{

    if (SpawnInterval >= maxSpawnRate)
    {
        SpawnInterval -= 0.05f;
    }


}

void AEnemySpawn::DelayMapChange()
{
    MyGameInstance->Changemap();
    UE_LOG(LogTemp, Error, TEXT("Do it"));
}

void AEnemySpawn::SetDamageArea()
{
    TArray<AActor*> DamageAreas;

    // Finde alle Actors mit dem Tag "DamageArea"
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("DamageArea"), DamageAreas);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der Area-Akteure gefunden West: %d"), DamageAreas.Num());

    if (DamageAreas.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Keine DamageAreas gefunden!"));
        return;
    }

    // Alle Actors zu Beginn verstecken und deaktivieren
    for (AActor* DamageArea : DamageAreas)
    {
        if (DamageArea)
        {
            DamageArea->SetActorHiddenInGame(true);
            DamageArea->SetActorEnableCollision(false);
            DamageArea->SetActorTickEnabled(false);
        }
    }



    // Timer starten, um alle 15 Sekunden den n�chsten Actor zu aktivieren
    GetWorld()->GetTimerManager().SetTimer(ActivationTimer, [this, DamageAreas]()
        {
            if (CurrentLevelName != "Testmap" && !bTriggeredClearTimers)
            {

                if (DamageAreas.Num() == 0)
                {
                    UE_LOG(LogTemp, Warning, TEXT("DamageAreas ist leer, Timer wird gestoppt."));
                    GetWorld()->GetTimerManager().ClearTimer(ActivationTimer); // Timer abbrechen
                    return; // Keine weitere Ausf�hrung
                }

                if (DamageAreas.IsValidIndex(CurrentIndex) && !bTriggeredClearTimers)
                {
                    AActor* LastActivated = DamageAreas[CurrentIndex];
                    AActor* LastActivated2 = DamageAreas[CurrentIndex2];
                    ADamageZone* zone = Cast<ADamageZone>(LastActivated);
                    ADamageZone* zone1 = Cast<ADamageZone>(LastActivated2);
                    if (zone)
                    {
                        FTimerHandle collisionhandle;
                        zone->ActivateLight();
                        //GetWorld()->GetTimerManager().SetTimer(collisionhandle, zone, &ADamageZone::SetColllision, 5.0f, false);
                        FTimerHandle collisionhandlee;
                        zone1->ActivateLight();
                        //GetWorld()->GetTimerManager().SetTimer(collisionhandlee, zone1, &ADamageZone::SetColllision, 5.0f, false);
                        //zone->SetColllision();
                       // zone1->SetColllision();
                  
                        /*
                        zone->SetActorHiddenInGame(true); // Versteckt den Actor
                        zone->SetActorEnableCollision(false);
                        zone->SetActorTickEnabled(false);
                        zone1->SetActorHiddenInGame(true); // Versteckt den Actor
                        zone1->SetActorEnableCollision(false);
                        zone1->SetActorTickEnabled(false);*/
                    }
                }

                // N�chsten Actor zuf�llig aktivieren
                CurrentIndex = FMath::RandRange(0, DamageAreas.Num() - 1); // Zuf�lliger Index
                CurrentIndex2 = FMath::RandRange(0, DamageAreas.Num() - 1);
                UE_LOG(LogTemp, Log, TEXT("CurrentIndex: %d"), CurrentIndex);

                while (CurrentIndex == CurrentIndex2)
                {
                    CurrentIndex2 = FMath::RandRange(0, DamageAreas.Num() - 1);
                }

                if (!bTriggeredClearTimers) {
                    AActor* NextActivated = DamageAreas[CurrentIndex];
                    AActor* NextActivated2 = DamageAreas[CurrentIndex2];
                    ADamageZone* zone2 = Cast<ADamageZone>(NextActivated);
                    ADamageZone* zone22 = Cast<ADamageZone>(NextActivated2);
                    if (zone2) // �berpr�fe, ob der Cast erfolgreich war
                    {
                        zone2->CleanColllision();
                        zone22->CleanColllision();
                        /*
                        zone2->SetActorHiddenInGame(false); // Versteckt den Actor
                        zone2->SetActorEnableCollision(true);
                        zone2->SetActorTickEnabled(true);
                        zone22->SetActorHiddenInGame(false); // Versteckt den Actor
                        zone22->SetActorEnableCollision(true);
                        zone22->SetActorTickEnabled(true);*/
                    }
               }  
            }

        }, DamageArea_timer, true);
}

void AEnemySpawn::SpawnPortal()
{
    TArray<AActor*> portalSpawnPoint;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PortalSpawn"), portalSpawnPoint);

    // �berpr�fen, ob es mindestens einen Actor im Array gibt
    if (portalSpawnPoint.Num() > 0)
    {
        // Zugriff auf den ersten Actor im Array
        AActor* actorPortal = portalSpawnPoint[0];

        // Ruft die Position des Actors ab
        FVector spawnLocation = actorPortal->GetActorLocation();

        // Optional: Ruft die Rotation des Actors ab
        FRotator spawnRotation = actorPortal->GetActorRotation();

        // Beispiel: Spawnen eines Portals an der Position des Actors
        if (portal != nullptr)
        {
            GetWorld()->SpawnActor<APortal>(portal, spawnLocation, spawnRotation);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No actors found with the 'PortalSpawn' tag."));
    }
}

void AEnemySpawn::BlockEntrance()
{
    UWorld* World = GetWorld();
    if (World)
    {
        // Die aktuelle Position und Rotation des AEnemySpawn-Actors verwenden
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
        SpawnRotation.Yaw += 90.0f;

        // Das AEntranceArena-Objekt spawnen
        AEntranceArena* SpawnedArena = World->SpawnActor<AEntranceArena>(wallEntrance, SpawnLocation, SpawnRotation);

        if (SpawnedArena)
        {
            // Optional: Weitergehende Initialisierung des spawned Arena-Objekts
            UE_LOG(LogTemp, Warning, TEXT("AEntranceArena wurde erfolgreich an der Position gespawnt: %s"), *SpawnLocation.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Fehler beim Spawnen von AEntranceArena."));
        }
    }
}

bool AEnemySpawn::GetLVLisActive()
{
    return activatelvl;
}

void AEnemySpawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player overlapped with colliderSpawn!"));

        GetWorldTimerManager().SetTimer(UpdateIndexTimerHandle, this, &AEnemySpawn::StartCountWaypoint, 15.0f, true);
        StartSpawning(Enemy_SlowAF);
    }
}



// Tick: Wird jede Frame aufgerufen
void AEnemySpawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    
    
    timer += DeltaTime;

    distance = FVector::Dist(GetActorLocation(), characterPlayer->GetActorLocation());
   // UE_LOG(LogTemp, Warning, TEXT("Distance to Player: %f"), distance);
    if (distance <= 600.0f && !activatelvl)
    {
        GetWorldTimerManager().SetTimer(UpdateIndexTimerHandle, this, &AEnemySpawn::StartCountWaypoint, 15.0f, true);
        StartSpawning(Enemy_SlowAF);
        activatelvl = true;
        //GetWorld()->GetTimerManager().SetTimer(Block_ActivationTimer,this,  &AEnemySpawn::BlockEntrance, 5.0f,  false);
        characterPlayer->ActivateLvl();
    }

    // Bedingung: Clear Timer nach 7 Sekunden
    if (timer >= (timer_ChangeMap - 7.0f) && !bTriggeredClearTimers || characterPlayer->player_win == true)
    {
        
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
        GetWorld()->GetTimerManager().ClearTimer(ActivationTimer);
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

        TArray<AActor*> DamageAreas;

        // Finde alle Actors mit dem Tag "DamageArea"
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("DamageArea"), DamageAreas);
        UE_LOG(LogTemp, Log, TEXT("Anzahl der Area-Akteure gefunden West: %d"), DamageAreas.Num());

        for (AActor* DamageArea : DamageAreas)
        {
            if (DamageArea)
            {
                DamageArea->SetActorHiddenInGame(true);
                DamageArea->SetActorEnableCollision(false);
                DamageArea->SetActorTickEnabled(false);
            }
        }
        bTriggeredClearTimers = true; // Verhindert Wiederholung
       // characterPlayer->lvlStart = false;
        characterPlayer->SetPortalTrue();
    }

    // Bedingung: Map wechseln nach 10 Sekunden
    if (timer >= timer_ChangeMap && !bTriggeredChangeMap)
    {
        SpawnPortal();
        bTriggeredChangeMap = true;

       /* if (MyGameInstance) // Sicherheit: MyGameInstance pr�fen
        {
            MyGameInstance->Changemap();
        }
        bTriggeredChangeMap = true; // Verhindert Wiederholung*/
    }
}


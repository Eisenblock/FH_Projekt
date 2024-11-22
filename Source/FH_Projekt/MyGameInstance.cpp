// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "EnemySpawn.h"


// Hier kannst du Logik hinzufügen, falls du beim Start des Spiels oder beim Laden von Leveln noch etwas initialisieren musst

void UMyGameInstance::Changemap()
{
    if (GetWorld())
    {
        
        UGameplayStatics::OpenLevel(GetWorld(), "TestMap");
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Changemap failed: GetWorld() returned nullptr."));
    }
}

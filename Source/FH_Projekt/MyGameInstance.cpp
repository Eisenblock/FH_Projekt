// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>



// Hier kannst du Logik hinzufügen, falls du beim Start des Spiels oder beim Laden von Leveln noch etwas initialisieren musst

void UMyGameInstance::Changemap()
{
    UWorld* World = GetWorld();  // Use GetWorld() directly

    if (World)
    {
        UGameplayStatics::OpenLevel(World, "TestMap");
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Changemap failed: GetWorld() returned nullptr."));
    }
}

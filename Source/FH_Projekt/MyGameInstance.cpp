// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>



// Hier kannst du Logik hinzufügen, falls du beim Start des Spiels oder beim Laden von Leveln noch etwas initialisieren musst

void UMyGameInstance::Changemap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("TestMap")));
}

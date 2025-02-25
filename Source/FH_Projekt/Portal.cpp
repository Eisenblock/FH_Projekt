// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal.h"
#include "FH_ProjektCharacter.h"
#include "Components/BoxComponent.h"        
#include "GameFramework/Character.h"         
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"      

// Sets default values
APortal::APortal()
{
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    // Stelle sicher, dass das TriggerVolume für Overlaps eingestellt ist
    TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // StaticMesh-Komponente für das visuelle Mesh erstellen
    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    mesh->SetupAttachment(RootComponent);

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
    NiagaraComponent->Activate();
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (portalNext) {
        if (OtherActor && (OtherActor != this) && OtherComp)
        {
            // Stelle sicher, dass der Spieler der kollidierende Actor ist, bevor das Level gewechselt wird
            if (OtherActor->IsA(ACharacter::StaticClass()))  // oder spezifischer: (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
            {
                UGameplayStatics::OpenLevel(this, nameMap);
            }
        }
    }

    if (portalWin) {
        if (OtherActor && (OtherActor != this) && OtherComp)
        {
            if (OtherActor->ActorHasTag("Player")) {
                AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);
                character->PlayerWInGame();
            }
        }
    }
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal.h"
#include "Components/BoxComponent.h"        
#include "GameFramework/Character.h"         
#include "Kismet/GameplayStatics.h"      

// Sets default values
APortal::APortal()
{
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    // Stelle sicher, dass das TriggerVolume f�r Overlaps eingestellt ist
    TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // StaticMesh-Komponente f�r das visuelle Mesh erstellen
    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        // Stelle sicher, dass der Spieler der kollidierende Actor ist, bevor das Level gewechselt wird
        if (OtherActor->IsA(ACharacter::StaticClass()))  // oder spezifischer: (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
        {
            FTimerHandle UnusedHandle;
            GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [this]()
                {
                    UGameplayStatics::OpenLevel(this, FName(TEXT("TestMap2")));
                }, 0.5f, false);
        }
    }
}

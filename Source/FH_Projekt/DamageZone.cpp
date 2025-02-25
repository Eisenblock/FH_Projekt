// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageZone.h"
#include "FH_ProjektCharacter.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADamageZone::ADamageZone()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bCanEverTick = true;

    // Collision Box erstellen
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // Event-Bindings
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnOverlapEnd);
    // Erstelle die Niagara-Komponente
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(RootComponent);

    PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLightComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{

    Super::BeginPlay();

    // SetActorHiddenInGame(true);
    ActiveNiagaraComponent = nullptr;

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnOverlapEnd);
    NiagaraComponent->Deactivate();
}

// Called every frame
void ADamageZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADamageZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);
        // Schaden-Timer starten
        character->GetDmg(DamageAmount);
        GetWorld()->GetTimerManager().SetTimer(DamageTimer, [this, character]()
            {
                // Methode GetDmg auf dem Player-Character aufrufen
                character->GetDmg(DamageAmount);

                // Niagara Effekt abspielen
               /* if (NiagaraEffect)
                {
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                        GetWorld(),
                        NiagaraEffect,
                        character->GetActorLocation(), // Effekt wird am Charakter abgespielt
                        FRotator::ZeroRotator
                    );
                }*/
            }, DamageInterval, true);


    }
}

void ADamageZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        // Schaden-Timer stoppen
        GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
    }
}

void ADamageZone::ApplyDamage(AActor* PlayerActor)
{
}

void ADamageZone::SetActorVisible()
{
    SetActorHiddenInGame(false);
}

void ADamageZone::SetColllision()
{
    SetActorHiddenInGame(false); // Versteckt den Actor
    SetActorEnableCollision(true);
    SetActorTickEnabled(true);
    NiagaraComponent->Activate();
}

void ADamageZone::CleanColllision()
{
    SetActorHiddenInGame(true); // Versteckt den Actor
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
}

void ADamageZone::ActivateLight()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
    NiagaraComponent->Deactivate();
    PointLightComponent->Activate();
    FTimerHandle timerCollision;
    GetWorld()->GetTimerManager().SetTimer(timerCollision, this, &ADamageZone::SetColllision, 3.0f, false);
}

void ADamageZone::DeActiveLight()
{
    PointLightComponent->Deactivate();

}

void ADamageZone::ActivateTrap()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
    NiagaraComponent->Deactivate();
    PointLightComponent->Activate();
    FTimerHandle timerCollision;
    GetWorld()->GetTimerManager().SetTimer(timerCollision, this, &ADamageZone::SetColllision, 1.0f, false);
}


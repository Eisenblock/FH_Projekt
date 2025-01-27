// Fill out your copyright notice in the Description page of Project Settings.


#include "GetLife.h"
#include "FH_ProjektCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Components/BoxComponent.h" // Include for UBoxComponent

// Sets default values
AGetLife::AGetLife()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Static Mesh component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	// Create the Collision Box component
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(StaticMesh);

	// Optionally set collision properties
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	//CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGetLife::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGetLife::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGetLife::OnOverlapBegin);
}

// Called every frame
void AGetLife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Time = GetWorld()->GetTimeSeconds();

	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Yaw += DeltaTime * 20.0f; // Rotiert mit 20 Grad/Sekunde

	// Verwende eine Interpolation für eine fließende Bewegung
	//FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0f);
	SetActorRotation(TargetRotation);

	// Hoch- und Runterbewegung (Sinus-Welle)
	FVector NewLocation = GetActorLocation();
	float Amplitude = 0.5f; // Amplitude der Bewegung
	float Frequency = 3.0f; // Frequenz der Bewegung (wie schnell)
	NewLocation.Z += Amplitude * FMath::Sin(Frequency * Time); // Wellenbewegung
	SetActorLocation(NewLocation);
}

void AGetLife::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		if (heal) {
		// Call the method to perform action
		AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);
		character->GetLife(life_amount);
		}
		
		if (bomb) {	
			// Array zum Speichern aller gefundenen Akteure
			// Array zum Speichern aller gefundenen Akteure
		TArray<AActor*> FoundEnemies;

		// Alle Akteure mit dem Tag "Enemy" finden
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), FoundEnemies);

		// Schleife durch alle gefundenen Gegner
		for (AActor* EnemyActor : FoundEnemies)
		{
			// Sicherheitsprüfung: Überprüfen, ob der Actor tatsächlich ein AEnemy ist
			AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
			if (Enemy)
			{
				// Abstand zwischen dem übergebenen Actor und dem aktuellen Gegner berechnen
				float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());

				// Wenn der Abstand kleiner oder gleich 100 ist, rufe die GetDmg-Methode auf
				if (Distance <= 4000.0f)
				{
					Enemy->GetDmgEnemy(Enemy->life,Enemy->GetActorLocation(), Enemy->GetActorLocation()); // Übergibt die Lebenspunkte an den Gegner
				}
			}
		}
		

		}

		Destroy();
	
	}
}

void AGetLife::PerformActionOnOverlap()
{
}



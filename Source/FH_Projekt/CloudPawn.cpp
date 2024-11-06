#include "CloudPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ACloudPawn::ACloudPawn()
{
	// Set this pawn to call Tick() every frame.  
	PrimaryActorTick.bCanEverTick = true;

	// Erstelle und setze die CapsuleComponent als Collider
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;  // Setze die Capsule als Root-Komponente

	// Erstelle und setze die StaticMeshComponent als Child der Root-Komponente
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//StaticMeshComponent->CanEverAffectNavigation();
	//StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Registriere die Methode, die aufgerufen wird, wenn eine Überlappung mit der Capsule beginnt
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ACloudPawn::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACloudPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACloudPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACloudPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Überlappungs-Event: Wird aufgerufen, wenn ein Actor mit der CapsuleComponent überlappt
void ACloudPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("CloudPawn hat den Player berührt!"));
	// Überprüfe, ob der andere Actor den Tag "Player" hat
	if (OtherActor && OtherActor != this)  // Stelle sicher, dass der andere Actor existiert und nicht der CloudPawn selbst ist
	{
		if (OtherActor->ActorHasTag(TEXT("Player")))  // Überprüfe, ob der Actor den Tag "Player" hat
		{
			// Hier kannst du deine Logik hinzufügen, wenn der CloudPawn den Player überlappt
			UE_LOG(LogTemp, Warning, TEXT("CloudPawn hat den Player berührt!"));

			// Beispielaktion: Bewege den CloudPawn durch den Player hindurch
			// (Du könntest hier weitere Logik implementieren, um den CloudPawn durch den Player hindurch fliegen zu lassen)
		}
	}
}

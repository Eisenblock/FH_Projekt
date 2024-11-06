#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "CloudPawn.generated.h"

UCLASS()
class FH_PROJEKT_API ACloudPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACloudPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Wird aufgerufen, wenn der CloudPawn mit einem anderen Actor überlappt
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistik")
	UStaticMeshComponent* StaticMeshComponent;
};

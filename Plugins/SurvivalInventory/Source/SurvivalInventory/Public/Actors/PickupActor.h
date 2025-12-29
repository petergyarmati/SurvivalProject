// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PickupActor.generated.h"

class UItemDefinition;

UCLASS()
class SURVIVALINVENTORY_API APickupActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	APickupActor();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// The Data Asset this pickup represents
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_ItemDef, Category = "Settings")
	TObjectPtr<const UItemDefinition> ItemDef;

	// Quantity (e.g., a stack of 5 arrows)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Settings")
	int32 Quantity = 1;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	virtual void BeginPlay() override;
    
	// Validates ItemDef and starts loading the mesh
	UFUNCTION()
	void OnRep_ItemDef();

	// Callback when mesh is loaded from Soft Pointer
	void OnMeshLoaded();

	// Shared resource for async loading
	TSharedPtr<FStreamableHandle> MeshLoadHandle;

	// --- Interaction Interface ---
	virtual FText GetInteractionText_Implementation(const APlayerController* Interactor) override;
	virtual void OnInteract_Implementation(APlayerController* Interactor) override;

};

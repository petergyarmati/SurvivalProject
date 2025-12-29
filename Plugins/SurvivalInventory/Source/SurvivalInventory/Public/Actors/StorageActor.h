// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "StorageActor.generated.h"

UCLASS()
class SURVIVALINVENTORY_API AStorageActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AStorageActor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	// --- Interaction Interface ---
	virtual void OnInteract_Implementation(APlayerController* Interactor) override
	{
		// 1. Get Controller
		if (APlayerController* PC = Cast<APlayerController>(Interactor))
		{
			// 2. Client RPC to open UI
			// We pass 'this' (the storage actor) so the UI knows which inventory to display
			Client_OpenStorageUI(PC);
		}
	}

	UFUNCTION(Client, Reliable)
	void Client_OpenStorageUI(APlayerController* PC);
};

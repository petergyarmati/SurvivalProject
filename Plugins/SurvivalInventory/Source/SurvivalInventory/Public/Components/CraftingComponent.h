// Copyright Peter Gyarmati (Bitrose Studio)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/FastArray.h"
#include "CraftingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCraftingQueueChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCraftingComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Enable Ticking to process the queue
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Replicated)
	FCraftingQueue CraftingQueue;

	// Reference to the Sibling Inventory (set in BeginPlay)
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> LinkedInventory;

	// --- Server API ---

	// 1. Player clicks "Craft" in UI
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RequestCraft(const UCraftingRecipe* Recipe, int32 Amount);

	// 2. Player clicks "X" on a queued item
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CancelJob(int32 JobID);

private:
	void ProcessCurrentJob(float DeltaTime);
	void FinishCraftingItem(FCraftingJob& Job);
    
	// Internal ID generator
	int32 NextJobID = 0;
};

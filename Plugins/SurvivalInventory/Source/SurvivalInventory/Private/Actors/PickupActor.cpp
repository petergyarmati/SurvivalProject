// Copyright Peter Gyarmati (Bitrose Studio)


#include "Actors/PickupActor.h"

#include "Components/InventoryComponent.h"
#include "Data/ItemDefinition.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"


APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APickupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemDef);
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APickupActor::OnRep_ItemDef()
{
	if (!ItemDef || ItemDef->PickupMesh.IsNull()) return;

	// 1. Check if already loaded
	if (UStaticMesh* LoadedMesh = ItemDef->PickupMesh.Get())
	{
		MeshComp->SetStaticMesh(LoadedMesh);
	}
	else
	{
		// 2. Request Async Load
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		MeshLoadHandle = Streamable.RequestAsyncLoad(
			ItemDef->PickupMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &APickupActor::OnMeshLoaded)
		);
	}
}

void APickupActor::OnMeshLoaded()
{
	if (ItemDef && ItemDef->PickupMesh.Get())
	{
		MeshComp->SetStaticMesh(ItemDef->PickupMesh.Get());
	}
}

FText APickupActor::GetInteractionText_Implementation(const APlayerController* Interactor)
{
	return IInteractionInterface::GetInteractionText_Implementation(Interactor);
}

void APickupActor::OnInteract_Implementation(APlayerController* Interactor)
{
	// Server-side logic
	if (HasAuthority())
	{
		if (UInventoryComponent* Inventory = Interactor->FindComponentByClass<UInventoryComponent>())
		{
			// Try to add the item
			// Note: You need a TryAddItem function that returns remaining count
			int32 Leftover = Inventory->TryAddItem(ItemDef, Quantity);

			if (Leftover == 0)
			{
				Destroy(); // Fully picked up
			}
			else
			{
				Quantity = Leftover; // Inventory full, update quantity on ground
			}
		}
	}
}


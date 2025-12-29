// Copyright Peter Gyarmati (Bitrose Studio)


#include "Actors/StorageActor.h"

#include "Components/InventoryComponent.h"


AStorageActor::AStorageActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComp->SetupAttachment(RootComponent);
	
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AStorageActor::Client_OpenStorageUI_Implementation(APlayerController* PC)
{
}


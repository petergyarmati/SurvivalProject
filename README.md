# SurvivalInventory: UE5 Replicated Survival Inventory System

**SurvivalInventory** is a modular, network-optimized inventory and crafting architecture designed for Unreal Engine 5 C++. It prioritizes low bandwidth usage and memory efficiency, making it suitable for large-scale multiplayer survival games.

## Key Features

* **Network Efficient:** Uses `FFastArraySerializer` to replicate only modified slots, minimizing bandwidth overhead.
* **Memory Optimized:** Items rely on `TSoftObjectPtr` and a centralized **Icon Manager Subsystem** to async load assets only when needed (no massive texture arrays in VRAM). *Icon Manager Subsystem not implemented yet.*
* **Flyweight Pattern:** Strict separation between Static Data (`UItemDefinition`) and Instance Data (`FInventorySlot`).
* **Modular Containers:** A single `UInventoryComponent` handles Player Backpacks, Storage Chests, and Hotbars via gameplay tag filtering.
* **Robust Crafting:** Queue-based crafting system with time-stamped replication (predictive UI) to prevent network spam.
* **Event-Driven UI:** Decoupled UI updates using Delegates—no binding to `Tick`.

## Architecture

### 1. Data Structure (Flyweight)
We avoid heavy `UObject` overhead for inventory items.
* **Static Data (`UItemDefinition`):** A `UPrimaryDataAsset` containing the Name, Weight, Icon (Soft Ptr), and Gameplay Tags (Behavior).
* **Instance Data (`FInventorySlot`):** A lightweight struct containing the Item ID, Quantity, and dynamic data (Durability/Ammo).

### 2. Networking Strategy
* **Inventory:** Utilizes `NetDeltaSerialize` to synchronize the inventory array.
* **Crafting:** Replicates `ServerStartTime` instead of `CurrentProgress`. The client calculates progress locally: `(GetTimeSeconds() - StartTime) / Duration`.

### 3. Asynchronous Loading
To support low-end hardware ("Potato PCs"), icons are not hard referenced.

*Not implemented yet:*
* **Icon Subsystem:** A `GameInstanceSubsystem` manages requests for icons.
* **Lifecycle:** Caches loaded textures in a map and clears them when the inventory closes to free memory.
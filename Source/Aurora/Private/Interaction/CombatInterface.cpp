// Copyright by wcyiming


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.

int32 ICombatInterface::GetPlayerLevel() {
    return 0;
}

FVector ICombatInterface::GetCombatSocketLocation(const FGameplayTag& MontageTag) {
    return FVector();
}

// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuroraGameplayAbility.h"
#include "AuroraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UAuroraDamageGameplayAbility : public UAuroraGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
	
};

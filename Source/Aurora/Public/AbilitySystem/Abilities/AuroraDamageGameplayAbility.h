// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuroraGameplayAbility.h"
#include "Interaction/CombatInterface.h"


#include "AuroraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UAuroraDamageGameplayAbility : public UAuroraGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
	
};

// Copyright by wcyiming


#include "AbilitySystem/AuroraAbilitySystemComponent.h"

void UAuroraAbilitySystemComponent::AbilityActorInfoSet() {
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuroraAbilitySystemComponent::ClientEffectApplied);
}

void UAuroraAbilitySystemComponent::ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) {
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

// Copyright by wcyiming


#include "Player/AuroraPlayerState.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"


AAuroraPlayerState::AAuroraPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UAuroraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuroraAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f; // Set a high update frequency for player state updates
}

UAbilitySystemComponent* AAuroraPlayerState::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

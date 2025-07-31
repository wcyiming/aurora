// Copyright by wcyiming


#include "Player/AuroraPlayerState.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Net/UnrealNetwork.h"


AAuroraPlayerState::AAuroraPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UAuroraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuroraAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f; // Set a high update frequency for player state updates
}

void AAuroraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuroraPlayerState, Level);
	DOREPLIFETIME(AAuroraPlayerState, XP);
	DOREPLIFETIME(AAuroraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuroraPlayerState, SpellPoints);
}

void AAuroraPlayerState::OnRep_Level(int32 OldLevel) {
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AAuroraPlayerState::OnRep_XP(int32 OldXP) {
}

void AAuroraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints) {
}

void AAuroraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints) {
}

UAbilitySystemComponent* AAuroraPlayerState::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

// Copyright by wcyiming


#include "AbilitySystem/Abilities/AuroraGameplayAbility.h"

FString UAuroraGameplayAbility::GetDescription(int32 Level) {
	return FString();
}

FString UAuroraGameplayAbility::GetNextLevelDescription(int32 Level) {
	return FString();
}

FString UAuroraGameplayAbility::GetLockedDescription(int32 Level) {
	return FString();
}

float UAuroraGameplayAbility::GetManaCost(float InLevel) const {
	return 0.0f;
}

float UAuroraGameplayAbility::GetCooldown(float InLevel) const {
	return 0.0f;
}

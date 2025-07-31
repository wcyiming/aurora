// Copyright by wcyiming


#include "AuroraAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "AuroraGameplayTags.h"

UAuroraAssetManager& UAuroraAssetManager::Get() {
	check(GEngine);

	UAuroraAssetManager* AuroraAssetManager = Cast<UAuroraAssetManager>(GEngine->AssetManager);
	return *AuroraAssetManager;
}

void UAuroraAssetManager::StartInitialLoading() {
	Super::StartInitialLoading();
	// Initialize Gameplay Tags
	FAuroraGameplayTags::InitializeNativeGameplayTags();
	// Initialize Ability System Globals
	UAbilitySystemGlobals::Get().InitGlobalData();
}

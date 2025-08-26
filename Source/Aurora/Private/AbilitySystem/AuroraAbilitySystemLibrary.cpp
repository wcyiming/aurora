// Copyright by wcyiming


#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuroraHUD.h"
#include "UI/WidgetController/AuroraWidgetController.h"

#include "Interaction/CombatInterface.h"
#include "Engine/OverlapResult.h" 
#include "Player/AuroraPlayerState.h"
#include "Game/AuroraGameModeBase.h"
#include "AuroraAbilityTypes.h"


bool UAuroraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuroraHUD*& OutAuraHUD) {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0)) {
		OutAuraHUD = Cast<AAuroraHUD>(PC->GetHUD());
		if (OutAuraHUD) {
			AAuroraPlayerState* PS = PC->GetPlayerState<AAuroraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuroraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject) {
	FWidgetControllerParams WCParams;
	AAuroraHUD* AuroraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuroraHUD)) {
		return AuroraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuroraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject) {
	FWidgetControllerParams WCParams;
	AAuroraHUD* AuroraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuroraHUD)) {
		return AuroraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UAuroraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC) {
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuroraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass) {
	// add startup abilities to character
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities) {
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities) {
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor())) {
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuroraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject) {
	const AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuroraGameMode == nullptr) return nullptr;
	return AuroraGameMode->CharacterClassInfo;
}

bool UAuroraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FAuroraGameplayEffectContext* AuraEffectContext = static_cast<const FAuroraGameplayEffectContext*>(EffectContextHandle.Get())) {
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuroraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FAuroraGameplayEffectContext* AuraEffectContext = static_cast<const FAuroraGameplayEffectContext*>(EffectContextHandle.Get())) {
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuroraAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit) {
	if (FAuroraGameplayEffectContext* AuroraEffectContext = static_cast<FAuroraGameplayEffectContext*>(EffectContextHandle.Get())) {
		AuroraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuroraAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit) {
	if (FAuroraGameplayEffectContext* AuroraEffectContext = static_cast<FAuroraGameplayEffectContext*>(EffectContextHandle.Get())) {
		AuroraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuroraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin) {
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps) {
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor())) {
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UAuroraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor) {
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

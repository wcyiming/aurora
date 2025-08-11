// Copyright by wcyiming


#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuroraHUD.h"
#include "UI/WidgetController/AuroraWidgetController.h"
#include "Player/AuroraPlayerState.h"
#include "Game/AuroraGameModeBase.h"


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

void UAuroraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC) {
	// add startup abilities to character
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities) {
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuroraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject) {
	const AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuroraGameMode == nullptr) return nullptr;
	return AuroraGameMode->CharacterClassInfo;
}

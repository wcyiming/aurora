// Copyright by wcyiming


#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuroraHUD.h"
#include "UI/WidgetController/AuroraWidgetController.h"
#include "Player/AuroraPlayerState.h"


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

// Copyright by wcyiming


#include "UI/WidgetController/AuroraWidgetController.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "Player/AuroraPlayerController.h"
#include "Player/AuroraPlayerState.h"

void UAuroraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params) {
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;

}

void UAuroraWidgetController::BroadcastInitialValues() {

}

void UAuroraWidgetController::BindCallbacksToDependencies() {

}

UAuroraAttributeSet* UAuroraWidgetController::GetAuroraAS() {
	if (AuroraAttributeSet == nullptr) {
		AuroraAttributeSet = Cast<UAuroraAttributeSet>(AttributeSet);
	}
	return AuroraAttributeSet;
}

AAuroraPlayerController* UAuroraWidgetController::GetAuroraPC() {
	if (AuroraPlayerController == nullptr) {
		AuroraPlayerController = Cast<AAuroraPlayerController>(GetOuter());
	}
	return Cast<AAuroraPlayerController>(PlayerController);
}

AAuroraPlayerState* UAuroraWidgetController::GetAuroraPS() {
	if (AuroraPlayerState == nullptr) {
		AuroraPlayerState = Cast<AAuroraPlayerState>(GetOuter());
	}
	return Cast<AAuroraPlayerState>(AuroraPlayerState);
}

UAuroraAbilitySystemComponent* UAuroraWidgetController::GetAuroraASC() {
	if (AbilitySystemComponent == nullptr) {
		AbilitySystemComponent = Cast<UAuroraAbilitySystemComponent>(GetOuter());
	}
	return Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent);
}

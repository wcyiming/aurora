// Copyright by wcyiming


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuroraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {
	check(AttributeInfo);
	for (auto& Pair : GetAuroraAS()->TagsToAttributes) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data) {
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	//GetAuroraPS()->OnAttributePointsChangedDelegate.AddLambda(
	//	[this](int32 Points) {
			//AttributePointsChangedDelegate.Broadcast(Points);
	//	}
	//);
}

void UAttributeMenuWidgetController::BroadcastInitialValues() {
	UAuroraAttributeSet* AS = CastChecked<UAuroraAttributeSet>(AttributeSet);

	for (auto& Pair : AS->TagsToAttributes) {
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	//AttributePointsChangedDelegate.Broadcast(GetAuroraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag) {
	//UAuroraAbilitySystemComponent* AuraASC = CastChecked<UAuroraAbilitySystemComponent>(AbilitySystemComponent);
	//AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const {
	FAuroraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

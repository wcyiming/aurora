// Copyright by wcyiming


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues() {
	OnHealthChanged.Broadcast(GetAuroraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuroraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuroraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuroraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies() {


	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuroraAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuroraAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuroraAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuroraAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (GetAuroraASC()) {


		GetAuroraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags) {
				for (const FGameplayTag& Tag : AssetTags) {
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False

					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag)) {
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

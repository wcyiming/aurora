// Copyright by wcyiming


#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "AuroraGameplayTags.h"

#include "Net/UnrealNetwork.h"

UAuroraAttributeSet::UAuroraAttributeSet() {

	const FAuroraGameplayTags& GameplayTags = FAuroraGameplayTags::Get();

	/* Primary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

}

void UAuroraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UAuroraAttributeSet, Health);            Different: REPNOTIFY_Always change the value even if it is the same as the old value, FGameplayAttributeData is struct, UE can't detect the change automatically
	// And if we setting it, we may want to respond to the act of setting it, no matter the value is changed or not.
	
	// Primary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Vital attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Mana, COND_None, REPNOTIFY_Always);


}

// two ways to clamp the value of attributes, only use for clamp the base value. The current value is still will be set by the GameplayEffect, which will over MaxHealth or MaxMana, that we must handle in the PostGameplayEffectExecute function.
void UAuroraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}

}
// Another way, recommanded by Epic, can do other logic due to the param Data.
void UAuroraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UAuroraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const {
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()) {
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr) {
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController) {
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


void UAuroraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Health, OldHealth);
}

void UAuroraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuroraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Mana, OldMana);
}

void UAuroraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, MaxMana, OldMaxMana);
}


// Primary attributes

void UAuroraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Strength, OldStrength);
}

void UAuroraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Intelligence, OldIntelligence);
}

void UAuroraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Resilience, OldResilience);
}

void UAuroraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Vigor, OldVigor);
}

void UAuroraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Armor, OldArmor);
}

void UAuroraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuroraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, BlockChance, OldBlockChance);
}

void UAuroraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuroraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuroraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuroraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuroraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, ManaRegeneration, OldManaRegeneration);
}


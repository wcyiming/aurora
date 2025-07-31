// Copyright by wcyiming


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth() {
	VigorCaptureDef.AttributeToCapture = UAuroraAttributeSet::GetVigorAttribute();
	VigorCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorCaptureDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const {
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorCaptureDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	int32 PlayerLevel = 1;
	//if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>()) {
	//	PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	//}
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	PlayerLevel = CombatInterface ? CombatInterface->GetPlayerLevel() : 1;

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}

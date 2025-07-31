// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuroraWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
struct FAuroraAttributeInfo;
struct FGameplayTag;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuroraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURORA_API UAttributeMenuWidgetController : public UAuroraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	//UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	//FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};

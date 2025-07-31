// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "AuroraHUD.generated.h"


class UAuroraUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAttributeSet;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
//class USpellMenuWidgetController;

/**
 * 
 */
UCLASS()
class AURORA_API AAuroraHUD : public AHUD
{
	GENERATED_BODY()

public:



	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);


private:

	UPROPERTY()
	TObjectPtr<UAuroraUserWidget>  OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuroraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	//UPROPERTY()
	//TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	
};

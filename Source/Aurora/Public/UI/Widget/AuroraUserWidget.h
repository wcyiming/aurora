// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuroraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UAuroraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};

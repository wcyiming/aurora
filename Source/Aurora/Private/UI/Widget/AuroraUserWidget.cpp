// Copyright by wcyiming


#include "UI/Widget/AuroraUserWidget.h"

void UAuroraUserWidget::SetWidgetController(UObject* InWidgetController) {
	if (InWidgetController) {
		WidgetController = InWidgetController;
		WidgetControllerSet();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("AuroraUserWidget: Attempted to set a null WidgetController!"));
	}
}


// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuroraInputConfig.h"
#include "AuroraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UAuroraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuroraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuroraInputComponent::BindAbilityActions(const UAuroraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc) {
	check(InputConfig);

	for (const FAuroraInputAction& Action : InputConfig->AbilityInputActions) {
		if (Action.InputAction && Action.InputTag.IsValid()) {
			if (PressedFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
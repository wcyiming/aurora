// Copyright by wcyiming

#pragma once

#include "GameplayEffectTypes.h"
#include "AuroraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuroraGameplayEffectContext : public FGameplayEffectContext {
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const {
		return StaticStruct();
	}

	virtual FAuroraGameplayEffectContext* Duplicate() const {
		FAuroraGameplayEffectContext* NewContext = new FAuroraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult()) {
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};

template<>
struct TStructOpsTypeTraits<FAuroraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuroraGameplayEffectContext> {
	enum {
		WithNetSerializer = true,
		WithCopy = true
	};
};
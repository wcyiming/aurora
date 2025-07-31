// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "Character/AuroraCharacterBase.h"
#include "AuroraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API AAuroraCharacter : public AAuroraCharacterBase
{
	GENERATED_BODY()
public:
	AAuroraCharacter();
	virtual void PossessedBy(AController* NewControllern) override;

	virtual void OnRep_PlayerState() override;

	// Combat interface
	virtual int32 GetPlayerLevel() override;

private:
	virtual void InitAbilityActorInfo() override;
	
};

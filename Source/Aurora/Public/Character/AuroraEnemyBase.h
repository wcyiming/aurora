// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "Character/AuroraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuroraEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API AAuroraEnemyBase : public AAuroraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuroraEnemyBase();

	// Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

protected:

	virtual void BeginPlay() override;

};

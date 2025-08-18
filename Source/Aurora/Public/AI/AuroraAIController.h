// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuroraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class AURORA_API AAuroraAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAuroraAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};

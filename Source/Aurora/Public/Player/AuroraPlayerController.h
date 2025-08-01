// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"

#include "AuroraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuroraInputConfig;
class UAuroraAbilitySystemComponent;
class USplineComponent;
class UNiagaraSystem;

enum class ETargetingStatus : uint8 {
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

/**
 * 
 */
UCLASS()
class AURORA_API AAuroraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuroraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void InitInput();

	
private:
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> AuroraMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& Value);

	void CursorTrace();
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> CurrentActor;
	FHitResult CursorHitResult;
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	void AbilityInputTagPresssed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAuroraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuroraAbilitySystemComponent> AuroraAbilitySystemComponent;

	UAuroraAbilitySystemComponent* GetASC();


	// Click to move
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	// Juidge if the click is a short press or a long press
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	// how close to the target player can be stopped
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();


};

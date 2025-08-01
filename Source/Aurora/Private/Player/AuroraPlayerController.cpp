// Copyright by wcyiming


#include "Player/AuroraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Input/AuroraInputConfig.h"
#include "Input/AuroraInputComponent.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuroraGameplayTags.h"

#include "Components/SplineComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"

AAuroraPlayerController::AAuroraPlayerController() {
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuroraPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void AAuroraPlayerController::CursorTrace() {
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	if (!CursorHitResult.bBlockingHit) return;

	LastActor = CurrentActor;
	if (IsValid(CursorHitResult.GetActor()) && CursorHitResult.GetActor()->Implements<UEnemyInterface>()) {
		CurrentActor = CursorHitResult.GetActor();
	}
	else {
		CurrentActor = nullptr;
	}

	if (LastActor != CurrentActor) {
		UnHighlightActor(LastActor);
		HighlightActor(CurrentActor);
	}
}

void AAuroraPlayerController::HighlightActor(AActor* InActor) {
	if (IsValid(InActor) && InActor->Implements<UEnemyInterface>()) {
		IEnemyInterface* Enemy = Cast<IEnemyInterface>(InActor);
		if (Enemy) {
			Enemy->HighlightActor();
		}
	}
}

void AAuroraPlayerController::UnHighlightActor(AActor* InActor) {
	if (IsValid(InActor) && InActor->Implements<UEnemyInterface>()) {
		IEnemyInterface* Enemy = Cast<IEnemyInterface>(InActor);
		if (Enemy) {
			Enemy->UnhighlightActor();
		}
	}
}

void AAuroraPlayerController::AbilityInputTagPresssed(FGameplayTag InputTag) {
	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuroraGameplayTags::Get().Player_Block_InputPressed)) {
		return;
	}
	if (InputTag.MatchesTagExact(FAuroraGameplayTags::Get().InputTag_LMB)) {
		if (IsValid(CurrentActor)) {
			TargetingStatus = CurrentActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		} else {
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
	
}

void AAuroraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag) {
	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuroraGameplayTags::Get().Player_Block_InputReleased)) {
		return;
	}
	if (!InputTag.MatchesTagExact(FAuroraGameplayTags::Get().InputTag_LMB)) {
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown) {
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn) {
			//if (IsValid(CurrentActor.GetObject()) && CurrentActor->Implements<UHighlightInterface>()) {
			//	IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			//}
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuroraGameplayTags::Get().Player_Block_InputPressed)) {
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination)) {
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints) {
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num() > 0) {
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
	
}

void AAuroraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag) {
	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuroraGameplayTags::Get().Player_Block_InputHeld)) {
		return;
	}
	if (!InputTag.MatchesTagExact(FAuroraGameplayTags::Get().InputTag_LMB)) {
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown) {
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	} else {
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHitResult.bBlockingHit) CachedDestination = CursorHitResult.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn()) {
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	
}

UAuroraAbilitySystemComponent* AAuroraPlayerController::GetASC() {
	if (AuroraAbilitySystemComponent == nullptr) {
		AuroraAbilitySystemComponent = Cast<UAuroraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuroraAbilitySystemComponent;
}

void AAuroraPlayerController::AutoRun() {
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn()) {
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius) {
			bAutoRunning = false;
		}
	}
}

void AAuroraPlayerController::BeginPlay() {
	Super::BeginPlay();

	InitInput();

}

void AAuroraPlayerController::InitInput()
{
	check(AuroraMappingContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(AuroraMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}


void AAuroraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	UAuroraInputComponent* AuroraInputComponent = CastChecked<UAuroraInputComponent>(InputComponent);

	AuroraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuroraPlayerController::Move);
		
	AuroraInputComponent->BindAbilityActions(InputConfig, this,
		&AAuroraPlayerController::AbilityInputTagPresssed,
		&AAuroraPlayerController::AbilityInputTagReleased,
		&AAuroraPlayerController::AbilityInputTagHeld);
}

void AAuroraPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn = GetPawn()) {
		ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}



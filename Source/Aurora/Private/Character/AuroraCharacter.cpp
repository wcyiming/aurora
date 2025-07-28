// Copyright by wcyiming


#include "Character/AuroraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuroraPlayerState.h"
#include "Player/AuroraPlayerController.h"
#include "UI/HUD/AuroraHUD.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"

AAuroraCharacter::AAuroraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuroraCharacter::PossessedBy(AController* NewControllern) {
	Super::PossessedBy(NewControllern);

	InitAbilityActorInfo();

}


void AAuroraCharacter::OnRep_PlayerState() {
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void AAuroraCharacter::InitAbilityActorInfo() {
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	check(AuroraPlayerState);
	AuroraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuroraPlayerState, this);
	AbilitySystemComponent = AuroraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuroraPlayerState->GetAttributeSet();

	if (AAuroraPlayerController* AuroraPlayerController = Cast<AAuroraPlayerController>(GetController())) {
		if (AAuroraHUD* AuroraHUD = Cast<AAuroraHUD>(AuroraPlayerController->GetHUD())) {
			AuroraHUD->InitOverlay(AuroraPlayerController, AuroraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
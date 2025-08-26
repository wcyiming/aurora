// Copyright by wcyiming


#include "Character/AuroraEnemyBase.h"
#include "Aurora/Aurora.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuroraUserWidget.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"

#include "AI/AuroraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AuroraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuroraEnemyBase::AAuroraEnemyBase() {
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuroraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;



	AttributeSet = CreateDefaultSubobject<UAuroraAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

}

void AAuroraEnemyBase::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuroraAIController = Cast<AAuroraAIController>(NewController);
	AuroraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuroraAIController->RunBehaviorTree(BehaviorTree);
	AuroraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuroraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);

}

void AAuroraEnemyBase::HighlightActor() {
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuroraEnemyBase::UnhighlightActor() {
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuroraEnemyBase::GetPlayerLevel() {
	return Level;
}

void AAuroraEnemyBase::Die() {
	SetLifeSpan(LifeSpan);
	if (AuroraAIController) AuroraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die();
}

void AAuroraEnemyBase::SetCombatTarget_Implementation(AActor* InCombatTarget) {
	CombatTarget = InCombatTarget;
}

AActor* AAuroraEnemyBase::GetCombatTarget_Implementation() const {
	return CombatTarget;
}

void AAuroraEnemyBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuroraAIController && AuroraAIController->GetBlackboardComponent()) {
		AuroraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}

}

void AAuroraEnemyBase::BeginPlay() {
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority()) {
		UAuroraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
}

void AAuroraEnemyBase::InitAbilityActorInfo() {
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority()) {
		InitializeDefaultAttributes();
	}

	if (UAuroraUserWidget* AuroraUserWidget = Cast<UAuroraUserWidget>(HealthBar->GetUserWidgetObject())) {
		AuroraUserWidget->SetWidgetController(this);
	}


	if (const UAuroraAttributeSet* AuraAS = Cast<UAuroraAttributeSet>(AttributeSet)) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuroraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuroraEnemyBase::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AAuroraEnemyBase::InitializeDefaultAttributes() const {
	UAuroraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

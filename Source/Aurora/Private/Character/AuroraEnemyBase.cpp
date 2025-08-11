// Copyright by wcyiming


#include "Character/AuroraEnemyBase.h"
#include "Aurora/Aurora.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuroraUserWidget.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"

#include "AuroraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuroraEnemyBase::AAuroraEnemyBase() {
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuroraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuroraAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

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
	Super::Die();
}

void AAuroraEnemyBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuroraEnemyBase::BeginPlay() {
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	UAuroraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
}

void AAuroraEnemyBase::InitAbilityActorInfo() {
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();

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

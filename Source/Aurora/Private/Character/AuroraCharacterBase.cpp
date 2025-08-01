// Copyright by wcyiming


#include "Character/AuroraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"

// Sets default values
AAuroraCharacterBase::AAuroraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuroraCharacterBase::InitAbilityActorInfo() {

}

void AAuroraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const {
	check(IsValid(GetAbilitySystemComponent()));
	if (GameplayEffectClass == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("GameplayEffectClass is null in ApplyEffectToSelf"));
		return;
	}
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuroraCharacterBase::InitializeDefaultAttributes() const {
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuroraCharacterBase::AddCharacterAbilities() {
	UAuroraAbilitySystemComponent* AuraASC = CastChecked<UAuroraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);
}

UAbilitySystemComponent* AAuroraCharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AAuroraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAuroraCharacterBase::GetCombatSocketLocation(const FGameplayTag& MontageTag) {
	if (Weapon) {
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	return FVector::ZeroVector;
}




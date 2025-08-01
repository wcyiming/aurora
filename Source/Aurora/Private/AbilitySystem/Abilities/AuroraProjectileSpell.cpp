// Copyright by wcyiming


#include "AbilitySystem/Abilities/AuroraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuroraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuroraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	FGameplayTag MyTag;
	SpawnProjectile(FVector::ZeroVector, MyTag, true, 0.f);
}

void UAuroraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride) {
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* Combat = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!Combat) {
		return;
	}
	const FVector SocketLocation = Combat->GetCombatSocketLocation(SocketTag);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) {
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	//SpawnTransform.SetRotation(Rotation.Quaternion());

	AAuroraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuroraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	Projectile->FinishSpawning(SpawnTransform);

	UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile: %s at Location: %s"), *Projectile->GetName(), *SocketLocation.ToString());
}

// Copyright by wcyiming


#include "Actor/AuroraEffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuroraAttributeSet.h"

AAuroraEffectActor::AAuroraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());

}


void AAuroraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuroraEffectActor::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAuroraEffectActor::EndOverlap);
	

}

void AAuroraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor)) {
		const UAuroraAttributeSet* AuroraAttributeSet = Cast<UAuroraAttributeSet>(AbilitySystemInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuroraAttributeSet::StaticClass()));
		UAuroraAttributeSet* MutableAuroraAttributeSet = const_cast<UAuroraAttributeSet*>(AuroraAttributeSet);
		MutableAuroraAttributeSet->SetHealth(AuroraAttributeSet->GetHealth() + 25.f);
		MutableAuroraAttributeSet->SetMana(AuroraAttributeSet->GetMana() + 25.f);
		Destroy();
	}
}

void AAuroraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}




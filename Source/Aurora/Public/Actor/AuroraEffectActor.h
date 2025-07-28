// Copyright by wcyiming

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuroraEffectActor.generated.h"

class USphereComponent;

UCLASS()
class AURORA_API AAuroraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuroraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

};

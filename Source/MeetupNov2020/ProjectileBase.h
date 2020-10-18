// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class MEETUPNOV2020_API AProjectileBase : public AActor
{
	GENERATED_BODY()

	friend class MN2Player;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Customization", meta = (DisplayName = "ProjectileColor", AllowPrivateAccess = true))
		FLinearColor m_Color;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Customization", meta = (DisplayName = "ProjectileScale", AllowPrivateAccess = true))
		FVector m_Scale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "ProjectileSpeed", ClampMin = "500", ClampMax = "2000", AllowPrivateAccess = true))
		float m_Speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MN2|Stats", meta = (DisplayName = "Damage", ClampMin="10", ClampMax="100", AllowPrivateAccess = true))
		float m_Damage;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Components", meta = (DisplayName = "ProjectileMovement", AllowPrivateAccess = true))
		class UProjectileMovementComponent* m_ProjectileMovementComp;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "MN2|Debug", meta = (DisplayName = "ProjectileMaterial", AllowPrivateAccess = true))
		class UMaterialInstanceDynamic* m_Material;

public:
	AProjectileBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnProjectileOverlapsDelegate_Impl(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UFUNCTION()
	void OnProjectileOverlapsDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	    OnProjectileOverlapsDelegate_Impl(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileEnemyBase.generated.h"

UCLASS()
class MEETUPNOV2020_API AProjectileEnemyBase : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	AProjectileEnemyBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MN2EnemyBase.h"
#include "MN2Asteroid.generated.h"

UCLASS()
class MEETUPNOV2020_API AMN2Asteroid : public AMN2EnemyBase
{
	GENERATED_BODY()
	
public:	
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual FEnemySpawnedEventArgs GetEnemySpawnedEventArgs() const override {
		return { GetClass(), GetActorLocation(), false };
	}
	virtual FEnemyKilledEventArgs GetEnemyKilledEventArgs() const override {
		return { GetClass(), GetActorLocation(), false, m_ScoreValue, .0f };
	}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MN2GameMode.h"
#include "GameFramework/Pawn.h"
#include "MN2Player.generated.h"

UCLASS()
class MEETUPNOV2020_API AMN2Player : public APawn
{
	GENERATED_BODY()

public:	
	AMN2Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveUpDown(float delta);
	void MoveLeftRight(float delta);

};

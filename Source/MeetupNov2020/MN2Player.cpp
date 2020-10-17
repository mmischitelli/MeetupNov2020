// Fill out your copyright notice in the Description page of Project Settings.


#include "MN2Player.h"

AMN2Player::AMN2Player()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMN2Player::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMN2Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMN2Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveUpDown", this, &AMN2Player::MoveUpDown);
	PlayerInputComponent->BindAxis("MoveLeftRight", this, &AMN2Player::MoveLeftRight);
}

void AMN2Player::MoveUpDown(float delta)
{
	
}

void AMN2Player::MoveLeftRight(float delta)
{
	
}

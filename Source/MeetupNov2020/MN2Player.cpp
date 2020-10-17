// Fill out your copyright notice in the Description page of Project Settings.


#include "MN2Player.h"

// Sets default values
AMN2Player::AMN2Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void AMN2Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMN2Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
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

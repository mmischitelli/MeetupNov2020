#include "ProjectileEnemyBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileEnemyBase::AProjectileEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileEnemyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	m_ProjectileMovementComp->SetVelocityInLocalSpace({ .0f, .0f, -1.0f });
}



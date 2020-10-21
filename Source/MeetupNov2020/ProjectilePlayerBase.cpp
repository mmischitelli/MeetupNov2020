#include "ProjectilePlayerBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectilePlayerBase::AProjectilePlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectilePlayerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	m_ProjectileMovementComp->SetVelocityInLocalSpace({ .0f, .0f, 1.0f });
}

void AProjectilePlayerBase::OnProjectileOverlapsDelegate_Impl(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_Emitter) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_Emitter, GetActorLocation());
	}
	Super::OnProjectileOverlapsDelegate_Impl(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

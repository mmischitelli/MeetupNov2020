#include "ProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	m_ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementLogic"));
	m_ProjectileMovementComp->ProjectileGravityScale = .0f;
}

void AProjectileBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const auto actorComp = GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (actorComp)
	{
		const auto rootMeshComp = static_cast<UStaticMeshComponent*>(actorComp);
		rootMeshComp->SetWorldScale3D(m_Scale);
		rootMeshComp->SetEnableGravity(false);
		rootMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnProjectileOverlapsDelegate);

		m_Material = rootMeshComp->CreateDynamicMaterialInstance(0);
		m_Material->SetVectorParameterValue(TEXT("Color"), m_Color);

		m_ProjectileMovementComp->MaxSpeed = m_Speed;
		m_ProjectileMovementComp->InitialSpeed = m_Speed;
	}
}

void AProjectileBase::OnProjectileOverlapsDelegate_Impl(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::ApplyDamage(OtherActor, m_Damage, GetInstigatorController(), this, nullptr);
	GetWorld()->DestroyActor(this);
}
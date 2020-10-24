#include "MN2Asteroid.h"
#include "MN2Player.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MeetupNov2020/MN2GameMode.h"


AMN2Asteroid::AMN2Asteroid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMN2Asteroid::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AMN2Asteroid::OnTakeAnyDamageDelegate);
}

void AMN2Asteroid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_Health = m_MaxHealth;
	m_SpawnLocation = Transform.GetLocation();
	m_CurrentMovementSpeed = UKismetMathLibrary::RandomFloatInRange(m_MinMovementSpeed, m_MaxMovementSpeed);
	
	const auto objComp = GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (objComp)
	{
		const auto meshComp = static_cast<UStaticMeshComponent*>(objComp);
		meshComp->OnComponentHit.AddDynamic(this, &AMN2Asteroid::OnCollisionHit);
		meshComp->SetWorldScale3D(m_Scale);		
	}
}

void AMN2Asteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto movementMagnitude = DeltaTime * m_CurrentMovementSpeed;
	const auto movementVector = FVector{ .0f, .0f, -movementMagnitude };

	AddActorWorldOffset(movementVector, true);
}

void AMN2Asteroid::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Bounce against walls
	if (IsValid(OtherActor))
	{
		if (OtherActor->GetClass()->IsChildOf(AMN2Player::StaticClass())) {
			UGameplayStatics::ApplyDamage(OtherActor, m_Health * .5f, nullptr, this, nullptr);
			UGameplayStatics::ApplyDamage(this, TNumericLimits<float>::Max(), OtherActor->GetInstigatorController(), OtherActor, nullptr);
		}
	}
}

void AMN2Asteroid::OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	m_Health = UKismetMathLibrary::FClamp(m_Health - Damage, 0, m_MaxHealth);
	if (m_Health <= .0f)
	{
		const auto gameMode = UGameplayStatics::GetGameMode(GetWorld());
		if (gameMode)
		{
			if (m_DeathEmitter) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathEmitter, GetActorLocation());
			}
			if (m_DeathSound) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_DeathSound, GetActorLocation());
			}
			const auto mn2GameMode = static_cast<AMN2GameMode*>(gameMode);
			mn2GameMode->IncrementScoreBy(m_ScoreValue);
		}
		GetWorld()->DestroyActor(this);
	}
}

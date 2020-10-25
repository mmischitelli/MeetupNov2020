#include "MN2EnemyBase.h"
#include "MN2Player.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MeetupNov2020/MN2GameModeBase.h"

AMN2EnemyBase::AMN2EnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_MeshComponent->SetEnableGravity(false);
	m_MeshComponent->SetCollisionProfileName(TEXT("EnemyCollision"));
	m_MeshComponent->SetGenerateOverlapEvents(true);
	m_MeshComponent->SetNotifyRigidBodyCollision(true);

	RootComponent = m_MeshComponent;
}

void AMN2EnemyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_Health = m_MaxHealth;
	m_SpawnLocation = Transform.GetLocation();
	m_CurrentMovementSpeed = FVector2D(UKismetMathLibrary::RandomFloatInRange(m_MinMovementSpeed, m_MaxMovementSpeed));

	m_MeshComponent->SetWorldScale3D(m_Scale);
	m_MeshComponent->OnComponentHit.AddDynamic(this, &AMN2EnemyBase::OnCollisionHit);

	OnTakeAnyDamage.AddDynamic(this, &AMN2EnemyBase::OnTakeAnyDamageDelegate);
}

void AMN2EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	static_cast<AMN2GameModeBase*>(UGameplayStatics::GetGameMode(GetWorld()))->RegisterEnemyEvents(this);
	OnEnemySpawned.Broadcast(GetEnemySpawnedEventArgs());
}

void AMN2EnemyBase::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsValid(OtherActor))
		return;
	
	// Bounce against walls
	if (OtherActor->ActorHasTag(TEXT("Bouncy")))
	{
		if (abs(m_CurrentMovementSpeed.X) > 10e-6) {
			m_CurrentMovementSpeed.X *= -1.0f;
		}
		if (abs(m_CurrentMovementSpeed.Y) > 10e-6) {
			m_CurrentMovementSpeed.Y *= -1.0f;
		}
	}

	// Self-destruct against player and do LOTS of damage
	if (OtherActor->GetClass()->IsChildOf(AMN2Player::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, m_Health * .5f, nullptr, this, nullptr);
		UGameplayStatics::ApplyDamage(this, TNumericLimits<float>::Max(), OtherActor->GetInstigatorController(), OtherActor, nullptr);
	}
}

void AMN2EnemyBase::OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	m_Health = UKismetMathLibrary::FClamp(m_Health - Damage, 0, m_MaxHealth);
	if (m_Health <= .0f)
	{
		const auto location = GetActorLocation();
		
		if (m_DeathEmitter) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathEmitter, location);
		}
		if (m_DeathSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_DeathSound, location);
		}

		const auto playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		auto data = GetEnemyKilledEventArgs();
		
		// Set score to 0 if this enemy was not killed by the player (suicide) or by one of its projectiles
		if (DamageCauser != playerPawn && DamageCauser->GetInstigator() != playerPawn) {
			data.ScoreValue = 0;
		}
		
		OnEnemyKilled.Broadcast(data);
		GetWorld()->DestroyActor(this);
	}
}

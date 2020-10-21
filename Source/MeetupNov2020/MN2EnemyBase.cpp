#include "MN2EnemyBase.h"
#include "MN2GameMode.h"
#include "MN2Player.h"
#include "ProjectileEnemyBase.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMN2EnemyBase::AMN2EnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	m_AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AnimationTimeline"));

	const auto updateFn = GET_FUNCTION_NAME_CHECKED(AMN2EnemyBase, OnSpawnAnimationUpdate);
	m_SpawnAnimationUpdateDelegate.BindUFunction(this, updateFn);
	const auto finishFn = GET_FUNCTION_NAME_CHECKED(AMN2EnemyBase, OnSpawnAnimationFinish);
	m_SpawnAnimationFinishDelegate.BindUFunction(this, finishFn);
}

void AMN2EnemyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

    m_Health = m_MaxHealth;
	m_CanMove = false;
	m_SpawnLocation = Transform.GetLocation();

	const auto objComp = GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (objComp) {
		static_cast<UStaticMeshComponent*>(objComp)->OnComponentHit.AddDynamic(this, &AMN2EnemyBase::OnBoxCollisionHit);
	}

	m_AnimationTimeline->SetTimelineLengthMode(TL_LastKeyFrame);
	m_AnimationTimeline->AddInterpFloat(m_SpawnAnimationCurve, m_SpawnAnimationUpdateDelegate);
	m_AnimationTimeline->SetTimelineFinishedFunc(m_SpawnAnimationFinishDelegate);

	OnTakeAnyDamage.AddDynamic(this, &AMN2EnemyBase::OnTakeAnyDamageDelegate);
}

void AMN2EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	const auto gameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (gameMode)
	{
		m_GameMode = static_cast<AMN2GameMode*>(gameMode);
		m_GameMode->IncrementNumOfEnemies();
	}

	FTimerHandle gunLogic;
	GetWorldTimerManager().SetTimer(gunLogic, this, &AMN2EnemyBase::AI_GunLogic, 0.1f, true, 0);

	m_MovementDirection = static_cast<EMovementDirection>(UKismetMathLibrary::RandomIntegerInRange(0, 1));
	m_CurrentMovementSpeed = UKismetMathLibrary::RandomFloatInRange(m_MinMovementSpeed, m_MaxMovementSpeed);
	m_SpawnLocation = GetActorLocation();
	m_AnimationTimeline->PlayFromStart();
}

void AMN2EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_CanMove)
		return;

	const auto movementMagnitude = DeltaTime * m_CurrentMovementSpeed * UKismetMathLibrary::SelectFloat(1.0f, -1.0f, m_MovementDirection == EMovementDirection::LeftToRight);
	const auto movementVector = FVector{ .0f, movementMagnitude, .0f };

	AddActorWorldOffset(movementVector, true);
}

void AMN2EnemyBase::_PlayGunfire() const
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_GunFireSound, GetActorLocation());
}

void AMN2EnemyBase::_FireProjectile() const
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AProjectileEnemyBase>(m_GunFireProjectile, GetActorLocation(), FRotator::ZeroRotator, params);
}

void AMN2EnemyBase::OnFireAction()
{	
	if (abs(m_GunFireDelay) < std::numeric_limits<float>::epsilon())
	{
		_PlayGunfire();
		_FireProjectile();
	}
	else if (m_GunFireDelay > .0f)
	{
		_FireProjectile();
		FTimerHandle sfxHandle;
		GetWorldTimerManager().SetTimer(sfxHandle, this, &AMN2EnemyBase::_PlayGunfire, m_GunFireDelay, false, 0);
	}
	else if (m_GunFireDelay < 0)
	{
		_PlayGunfire();
		FTimerHandle gfxHandle;
		GetWorldTimerManager().SetTimer(gfxHandle, this, &AMN2EnemyBase::_FireProjectile, -m_GunFireDelay, false, 0);
	}
}

void AMN2EnemyBase::OnSpawnAnimationUpdate(float delta)
{
	const auto finalLocation = m_SpawnLocation - FVector{ .0f, .0f, m_MovementOffsetAtSpawn };
	const auto currentAnimatedLocation = UKismetMathLibrary::VLerp(m_SpawnLocation, finalLocation, delta);
	SetActorLocation(currentAnimatedLocation);
}

void AMN2EnemyBase::OnSpawnAnimationFinish()
{
	m_CanMove = true;
}

void AMN2EnemyBase::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Bounce against walls
	if (IsValid(OtherActor))
	{
		if (OtherActor->ActorHasTag(TEXT("Bouncy"))) {
			m_MovementDirection = m_MovementDirection == EMovementDirection::LeftToRight ? EMovementDirection::RightToLeft : EMovementDirection::LeftToRight;
		}
		else if (OtherActor->GetClass()->IsChildOf(AMN2Player::StaticClass())) {
			UGameplayStatics::ApplyDamage(OtherActor, m_Health * .5f, nullptr, this, nullptr);
			UGameplayStatics::ApplyDamage(this, TNumericLimits<float>::Max(), OtherActor->GetInstigatorController(), OtherActor, nullptr);
		}
	}
}

void AMN2EnemyBase::OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	m_Health = UKismetMathLibrary::FClamp(m_Health - Damage, 0, m_MaxHealth);
	if (m_Health <= .0f)
	{
		if (m_DeathEmitter) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathEmitter, GetActorLocation());
		}
		if (m_DeathSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_DeathSound, GetActorLocation());
		}
		if (UKismetMathLibrary::RandomFloatInRange(.0f, 1.0f) <= m_HealthGlobeProbability) {
			GetWorld()->SpawnActor<AActor>(m_HealthGlobe, GetActorLocation(), FRotator::ZeroRotator, FActorSpawnParameters());
		}
		m_GameMode->IncrementNumOfEnemiesKilled();
		m_GameMode->IncrementScoreBy(m_ScoreValue);
		GetWorld()->DestroyActor(this);
	}
}

void AMN2EnemyBase::AI_GunLogic()
{	
	const auto player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!IsValid(player))
		return;
	
	if (abs(player->GetActorLocation().Y - GetActorLocation().Y) <= m_EngagementDistance)
	{
		if (!m_FireActionRepeat.IsValid()) {
			GetWorldTimerManager().SetTimer(m_FireActionRepeat, this, &AMN2EnemyBase::OnFireAction, m_FireRate, true, 0);
		}
	}
	else
	{
		if (m_FireActionRepeat.IsValid()) {
			GetWorldTimerManager().ClearTimer(m_FireActionRepeat);
		}
	}
}

#include "MN2EnemyShip.h"
#include "MN2Player.h"
#include "MeetupNov2020/Projectiles/ProjectileEnemyBase.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMN2EnemyShip::AMN2EnemyShip()
{
	PrimaryActorTick.bCanEverTick = true;

	m_AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AnimationTimeline"));

	const auto updateFn = GET_FUNCTION_NAME_CHECKED(AMN2EnemyShip, OnSpawnAnimationUpdate);
	m_SpawnAnimationUpdateDelegate.BindUFunction(this, updateFn);
	const auto finishFn = GET_FUNCTION_NAME_CHECKED(AMN2EnemyShip, OnSpawnAnimationFinish);
	m_SpawnAnimationFinishDelegate.BindUFunction(this, finishFn);
}

void AMN2EnemyShip::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_CanMove = false;
	m_AnimationTimeline->SetTimelineLengthMode(TL_LastKeyFrame);
	m_AnimationTimeline->AddInterpFloat(m_SpawnAnimationCurve, m_SpawnAnimationUpdateDelegate);
	m_AnimationTimeline->SetTimelineFinishedFunc(m_SpawnAnimationFinishDelegate);
}

void AMN2EnemyShip::BeginPlay()
{
	Super::BeginPlay();

	const auto initialDirection = UKismetMathLibrary::RandomIntegerInRange(0, 1) * 2 - 1; // get either -1 or +1
	m_CurrentMovementSpeed.X = initialDirection * UKismetMathLibrary::RandomFloatInRange(m_MinMovementSpeed, m_MaxMovementSpeed);
	
	m_AnimationTimeline->PlayFromStart();
}

void AMN2EnemyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!m_CanMove)
		return;

	const auto horizontalMovement = DeltaTime * m_CurrentMovementSpeed.X;
	AddActorWorldOffset({ .0f, horizontalMovement, .0f }, true);
}

void AMN2EnemyShip::StartFiring()
{
	if (!m_FireActionRepeat.IsValid()) {
		GetWorldTimerManager().SetTimer(m_FireActionRepeat, this, &AMN2EnemyShip::OnFireAction, m_FireRate, true, 0);
	}
}

void AMN2EnemyShip::StopFiring()
{
	if (m_FireActionRepeat.IsValid()) {
		GetWorldTimerManager().ClearTimer(m_FireActionRepeat);
	}
}

void AMN2EnemyShip::_PlayGunfire() const
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_GunFireSound, GetActorLocation());
}

void AMN2EnemyShip::_FireProjectile() const
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AProjectileEnemyBase>(m_GunFireProjectile, GetActorLocation(), FRotator::ZeroRotator, params);
}

void AMN2EnemyShip::OnFireAction()
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
		GetWorldTimerManager().SetTimer(sfxHandle, this, &AMN2EnemyShip::_PlayGunfire, m_GunFireDelay, false);
	}
	else if (m_GunFireDelay < 0)
	{
		_PlayGunfire();
		FTimerHandle gfxHandle;
		GetWorldTimerManager().SetTimer(gfxHandle, this, &AMN2EnemyShip::_FireProjectile, -m_GunFireDelay, false);
	}
}

void AMN2EnemyShip::OnSpawnAnimationUpdate(float delta)
{
	const auto finalLocation = m_SpawnLocation - FVector{ .0f, .0f, m_MovementOffsetAtSpawn };
	const auto currentAnimatedLocation = UKismetMathLibrary::VLerp(m_SpawnLocation, finalLocation, delta);
	SetActorLocation(currentAnimatedLocation);
}

void AMN2EnemyShip::OnSpawnAnimationFinish()
{
	m_CanMove = true;
}

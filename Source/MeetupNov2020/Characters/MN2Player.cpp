#include "MN2Player.h"
#include "MeetupNov2020/MN2GameModeBase.h"
#include "MeetupNov2020/Projectiles/ProjectilePlayerBase.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MeetupNov2020/Utils/MN2RateLimitedAction.h"

AMN2Player::AMN2Player()
    : m_kBounceFactor(3.0f)
    , m_kYawLeanOnMovement(5.0f)
    , m_kYawLeanInterpSpeed(250.0f)
    , m_kMovementDriftInterpSpeed(10.0f)
{
    PrimaryActorTick.bCanEverTick = true;

	m_AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AnimationTimeline"));

	const auto updateFn = GET_FUNCTION_NAME_CHECKED(AMN2Player, OnSpawnAnimationUpdate);
	m_SpawnAnimationUpdateDelegate.BindUFunction(this, updateFn);

	const auto finishFn = GET_FUNCTION_NAME_CHECKED(AMN2Player, OnSpawnAnimationFinish);
	m_SpawnAnimationFinishDelegate.BindUFunction(this, finishFn);
}

void AMN2Player::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_Health = m_MaxHealth;
	m_CanMove = false;
	m_SpawnLocation = Transform.GetLocation();

	m_PrimaryAction = NewObject<UMN2RateLimitedAction>(this);
	m_PrimaryAction->SetRate(m_FireRate);
	m_PrimaryAction->OnFireAction.AddWeakLambda(this, [&]() {
		SpawnProjectile(m_PrimaryFireProjectile, m_PrimaryFireSound, "PrimaryFireSpawnLoc");
	});

	m_SecondaryAction = NewObject<UMN2RateLimitedAction>(this);
	m_SecondaryAction->SetRate(m_FireRate);
	m_SecondaryAction->OnFireAction.AddWeakLambda(this, [&]() {
		SpawnProjectile(m_SecondaryFireProjectile, m_SecondaryFireSound, "SecondaryFireSpawnLoc");
	});

	const auto components = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "MeshRoot");
	if (components.Num() > 0) {
		m_MeshRoot = static_cast<UStaticMeshComponent*>(components[0]);
	}

	const auto boxComp = static_cast<UBoxComponent*>(RootComponent);
	if (boxComp) {
		boxComp->OnComponentHit.AddDynamic(this, &AMN2Player::OnBoxCollisionHit);
	}

	m_AnimationTimeline->SetTimelineLengthMode(TL_LastKeyFrame);
	m_AnimationTimeline->AddInterpFloat(m_SpawnAnimationCurve, m_SpawnAnimationUpdateDelegate);
	m_AnimationTimeline->SetTimelineFinishedFunc(m_SpawnAnimationFinishDelegate);

	OnTakeAnyDamage.AddDynamic(this, &AMN2Player::OnTakeAnyDamageDelegate);
}

void AMN2Player::BeginPlay()
{
	Super::BeginPlay();

	m_Yaw = GetActorRotation().Yaw;
	m_AnimationTimeline->PlayFromStart();
}

void AMN2Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_DeltaSecs = DeltaTime;

	SetActorLocation(GetActorLocation() + FVector{ .0f, m_CurrentMovementSpeed.X, .0f },true);
	SetActorLocation(GetActorLocation() + FVector{ .0f, .0f, m_CurrentMovementSpeed.Y }, true);

	const auto currentRotation = m_MeshRoot->GetRelativeRotation();
	const auto targetRotation = FRotator{ .0f, m_Yaw - m_CurrentMovementSpeed.X * m_kYawLeanOnMovement, .0f};
	const auto interpolatedRotation = UKismetMathLibrary::RInterpTo_Constant(currentRotation, targetRotation, DeltaTime, m_kYawLeanInterpSpeed);
	m_MeshRoot->SetRelativeRotation(interpolatedRotation);
}

void AMN2Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("FirePrimary"  , IE_Pressed , this, &AMN2Player::FirePrimary     );
	PlayerInputComponent->BindAction("FirePrimary"  , IE_Released, this, &AMN2Player::ReleasePrimary  );
	PlayerInputComponent->BindAction("FireSecondary", IE_Pressed , this, &AMN2Player::FireSecondary   );
	PlayerInputComponent->BindAction("FireSecondary", IE_Released, this, &AMN2Player::ReleaseSecondary);

	PlayerInputComponent->BindAxis("MoveUpDown"   , this, &AMN2Player::MoveUpDown   );
	PlayerInputComponent->BindAxis("MoveLeftRight", this, &AMN2Player::MoveLeftRight);
}

void AMN2Player::MoveUpDown(float delta)
{
	m_CurrentMovementSpeed.Y = ComputeMovementAmount(delta, m_CurrentMovementSpeed.Y);
}

void AMN2Player::MoveLeftRight(float delta)
{
	m_CurrentMovementSpeed.X = ComputeMovementAmount(delta, m_CurrentMovementSpeed.X);
}

void AMN2Player::FirePrimary()
{
	if (!m_CanMove)
		return;

	m_PrimaryAction->Start(GetWorld());
}

void AMN2Player::ReleasePrimary()
{
	m_PrimaryAction->Stop();
}

void AMN2Player::FireSecondary()
{
	if (!m_CanMove)
		return;

	m_SecondaryAction->Start(GetWorld());
}

void AMN2Player::ReleaseSecondary()
{
	m_SecondaryAction->Stop();
}

void AMN2Player::SpawnProjectile(TSubclassOf<AActor> actorToSpawn, USoundBase* soundFX, const FString& tag)
{
	const auto guns = GetComponentsByTag(USceneComponent::StaticClass(), *tag);
	for (UActorComponent* const gun : guns)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		params.Instigator = this;
		const auto gunLocation = static_cast<USceneComponent*>(gun)->GetComponentToWorld().GetLocation();
		GetWorld()->SpawnActor<AProjectilePlayerBase>(actorToSpawn, gunLocation, FRotator::ZeroRotator, params);

		if (soundFX != nullptr)
		{
			const float volumeMultiplier = 1.0f / guns.Num();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundFX, gunLocation, volumeMultiplier);
		}
	}
}

void AMN2Player::OnSpawnAnimationUpdate(float delta)
{
	const auto finalLocation = m_SpawnLocation + FVector{ .0f, .0f, m_MovementOffsetAtSpawn };
	const auto currentAnimatedLocation = UKismetMathLibrary::VLerp(m_SpawnLocation, finalLocation, delta);
	SetActorLocation(currentAnimatedLocation);

	const auto finalYawRotation = m_Yaw + 360.0f * m_NumSpinsAtSpawn;
	const auto currentAnimatedRotation = UKismetMathLibrary::RLerp(FRotator::ZeroRotator, FRotator{ .0f, finalYawRotation, .0f }, delta, false);
	m_MeshRoot->SetRelativeRotation(currentAnimatedRotation);
}

void AMN2Player::OnSpawnAnimationFinish()
{
	m_CanMove = true;
}

void AMN2Player::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Bounce against walls
	if(IsValid(OtherActor) && OtherActor->ActorHasTag(TEXT("Bouncy"))) {
		m_CurrentMovementSpeed = FVector2D{ Hit.Normal.Y, Hit.Normal.Z } * UKismetMathLibrary::GetAbs2D(m_CurrentMovementSpeed) * m_kBounceFactor;
	}
}

float AMN2Player::ComputeMovementAmount(float direction, float speed) const
{
	const auto targetSpeed = m_MaxMovementSpeed * direction * m_DeltaSecs;
	return UKismetMathLibrary::FInterpTo(speed, targetSpeed, m_DeltaSecs, m_kMovementDriftInterpSpeed);
}

void AMN2Player::OnTakeAnyDamageDelegate(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
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

		static_cast<AMN2GameModeBase*>(UGameplayStatics::GetGameMode(GetWorld()))->ShowGameOverScreen();
		GetWorld()->DestroyActor(this);
	}
}

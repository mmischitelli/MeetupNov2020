#include "MN2Asteroid.h"
#include "GameFramework/GameModeBase.h"

void AMN2Asteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto verticalMovement = -DeltaTime * m_CurrentMovementSpeed.Y;
	AddActorWorldOffset({ .0f, .0f, verticalMovement }, true);
}


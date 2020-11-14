#include "MN2RateLimitedAction.h"

void UMN2RateLimitedAction::Start(UWorld* world)
{
	m_World = world;
	m_ShouldStop = false;
	if (m_ActionHandle.IsValid())
		return;

	m_World->GetTimerManager().SetTimer(m_ActionHandle, this, &UMN2RateLimitedAction::_ExecuteAction, m_Rate, true, .0f);
}

void UMN2RateLimitedAction::Stop()
{
	m_ShouldStop = true;
}

void UMN2RateLimitedAction::_ExecuteAction()
{
	if (m_ShouldStop)
	{
		m_World->GetTimerManager().ClearTimer(m_ActionHandle);
		return;
	}

	OnFireAction.Broadcast();
}

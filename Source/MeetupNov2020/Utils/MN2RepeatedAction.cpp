#include "MN2RepeatedAction.h"

UMN2RepeatedAction* UMN2RepeatedAction::RepeatAction(AActor* owner, float timeBetweenActions, int32 totalActionCount)
{
	UMN2RepeatedAction* obj = NewObject<UMN2RepeatedAction>(owner);
	obj->m_World = owner->GetWorld();
	obj->TotalActionCount = totalActionCount;
	obj->TimeBetweenActions = timeBetweenActions;
	return obj;
}

void UMN2RepeatedAction::Start()
{
	NumPerformedActions = 0;
	if (NumPerformedActions < TotalActionCount)
	{
		if (m_World) {
			m_World->GetTimerManager().SetTimer(m_PerformActionHandle, this, &UMN2RepeatedAction::_PerformAction, TimeBetweenActions, true);
		}
	}
	else
	{
		OnFinished.Broadcast(NumPerformedActions);
		_EndTask();
	}
}

void UMN2RepeatedAction::_PerformAction()
{
	OnPerformAction.Broadcast(NumPerformedActions);
	++NumPerformedActions;
	if (NumPerformedActions >= TotalActionCount) {
		OnFinished.Broadcast(NumPerformedActions);
		_EndTask();
	}
}

void UMN2RepeatedAction::_EndTask()
{
	if (m_World) {
		m_World->GetTimerManager().ClearTimer(m_PerformActionHandle);
	}
}

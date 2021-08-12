/*
	File:						CEventManager.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/10/2012
	Purpose:				Handles and processes all events in the game
*/

#include "EventManager.h"

CEventManager* CEventManager::GetInstance()
{
	static CEventManager sInstance;
	return &sInstance;
}

void CEventManager::RegisterClient( EVENT eventID, IListener* pClient )
{
	//Error check to make sure this hasn't been registered already
	if( !pClient || AlreadyRegistered( eventID, pClient )) return;

	//creating a database of clients
	m_ClientDatabase.insert(make_pair( eventID, pClient ));
}

void CEventManager::UnregisterClient( EVENT eventID, IListener* pClient )
{
	//Iterator that will iterate through all of out clients
	pair<multimap<EVENT, IListener*>::iterator, multimap<EVENT, IListener*>::iterator> range;

	//Look for clients that can receive this event
	range = m_ClientDatabase.equal_range(eventID);

	//Go Through the list of clients that are able to receive this event
	for( multimap<EVENT, IListener*>::iterator mmIter = range.first; mmIter != range.second; mmIter++ )
	{
		//checking if the pointer is equal to the ciient
		if( (*mmIter).second == pClient )
		{
			//erasing the client from the list
			mmIter = m_ClientDatabase.erase( mmIter );
			break;
		}
	}
}

void CEventManager::UnregisterClientAll( IListener* pClient )
{
	multimap< string, IListener* >::iterator mmIter = m_ClientDatabase.begin();

	//clearing everything out
	while( mmIter != m_ClientDatabase.end() )
	{
		if( (*mmIter).second == pClient )
		{
			mmIter = m_ClientDatabase.erase( mmIter );
		}
		else
			mmIter++;
	}
}

bool CEventManager::HasEventTriggered( EVENT eventID )
{
	//checking to make sure that the event has not been 
	list<IEvent>::iterator iter = m_CurrentEvents.begin();

	for( ; iter != m_CurrentEvents.end(); ++iter )
	{
		if( (*iter).GetEventID() == eventID )
			return true;
	}

	//if not in queue
	return false;
}

void CEventManager::sendEvent( EVENT eventID, void* pData /*= NULL */ )
{
	//pushing my event to the end of the list
	IEvent newEvent( eventID, pData );

	m_CurrentEvents.push_back(newEvent);

}

void CEventManager::SendUniqueEvent( EVENT eventID, void* pData /*= NULL */ )
{
	//ignore the event if its already in the queue
	if( HasEventTriggered(eventID) )
		return;

	//put the event in queue
	sendEvent( eventID, pData );
}

void CEventManager::ProcessEvents()
{
	while( m_CurrentEvents.size() )
	{
		DispatchEvent( &m_CurrentEvents.front() );
		m_CurrentEvents.pop_front();
	}
}

void CEventManager::ClearEvents()
{
	m_CurrentEvents.clear();
}

void CEventManager::ShutdownEventSystem()
{
	m_ClientDatabase.clear();
}


//private utility functions
void CEventManager::DispatchEvent(IEvent* pEvent )
{
	//iterator that can iterate through every client that can hear this event
	pair<multimap<EVENT, IListener*>::iterator, multimap<EVENT, IListener*>::iterator> range;

	//find all clients that can receive this event
	range = m_ClientDatabase.equal_range(pEvent->GetEventID());

	for( multimap<EVENT, IListener*>::iterator mmIter = range.first; mmIter != range.second; mmIter++ )
		(*mmIter).second->HandleEvent( pEvent);  // pass the event to the client
}

bool CEventManager::AlreadyRegistered( EVENT eventID, IListener* pClient )
{
	bool bIsAlreadyRegistered = false;

	//iterator to iterate through out clients that can hear this event
	pair<multimap<EVENT, IListener*>::iterator, multimap<EVENT, IListener*>::iterator> range;

	//find all the clients that can receive this event
	range = m_ClientDatabase.equal_range(eventID);

	//going through the list of clients that are able to receive this event
	for( multimap<EVENT, IListener*>::iterator mmIter = range.first; mmIter != range.second; mmIter++ )
	{
		//check if the pointer is equal to the client
		if( (*mmIter).second == pClient )
		{
			bIsAlreadyRegistered = true;
			break;
		}
	}

	return bIsAlreadyRegistered;
}

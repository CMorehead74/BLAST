/*
	File:						CEventManager.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/10/2012
	Purpose:				Handles and processes all events in the game
*/

#pragma once

#include <list>
#include <map>
using namespace std;

#include "../SGD Wrappers/IListener.h"

#include "IEvent.h"

class CEventManager
{
private:

	//List of events that need to be processed
	list<IEvent>									m_CurrentEvents;

	//database that contains clients that can listen for events
	multimap<EVENT, IListener* >		m_ClientDatabase;

	//a couple of utility functions that ONLY this class will use
	//DO NOT TRY TO ACCESS ANYWHERE ELSE!!
	//Will search the database for an event and call the registered client EventHandler
	void DispatchEvent( IEvent* pEvent );
	bool AlreadyRegistered( EVENT eventID, IListener* pClient);

	//making sure that this is a secure singleton
	CEventManager(){ }
	~CEventManager(void){}
	CEventManager( const CEventManager&);
	CEventManager& operator=( const CEventManager& );

public:

	//singleton
	static CEventManager* GetInstance();

	//Adds clients to the database
	void RegisterClient( EVENT eventID, IListener* pClient );

	//unregisters the client for the specified event only
	void UnregisterClient( EVENT eventID, IListener* pClient );

	//removes the client from the database completely
	void UnregisterClientAll( IListener* pClient );

	//Checks if an event is already queued
	bool HasEventTriggered( EVENT eventID );

	//sends event that does not need to be processed till later on
	void sendEvent( EVENT eventID, void* pData = NULL );

	//sends an event that has not been triggered
	void SendUniqueEvent( EVENT eventID, void* pData = NULL );

	//Processes all the events in the list
	void ProcessEvents();

	//clears all the pending events
	void ClearEvents();

	//shuts it all down
	void ShutdownEventSystem();
};


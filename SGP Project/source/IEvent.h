/*
	File:						IEvent.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/10/2012
	Purpose:				Handles and processes all events in the game
*/

#pragma once

#include <string>
using namespace std;

//typedef for all the events we are going to use
typedef string EVENT;

class IEvent
{
private:
	//Event ID	
	EVENT											m_EventID;

	//able to store any type of variable but only one
	void*												m_pVariable;

public:
	IEvent(EVENT eventID, void* pParam = NULL )
	{
		m_EventID = eventID;
		m_pVariable = pParam;
	}

	virtual ~IEvent(void) {}

	//accessors
	EVENT GetEventID()				{ return m_EventID; }
	void* GetVariable()				{return m_pVariable; }
};


//////////////////////////////////////////////////////////////////////////
//	File	:	"IListener.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Base class for objects registered with CSGD_EventSystem to
//				handle event broadcasts.
///////////////////////////////////////////////////////////////////////////
#pragma once

//I edited this file to handle out IEVENT and not CEVENT
//-Schadrac Francois


// Forward declaration does not require header includes.
class IEvent;

// ABSTRACT BASE CLASS
class IListener
{
public:
	IListener(void) {}
	virtual ~IListener(void) {}

	//	Blue print function - MUST be defined in the derived class.
	//	Mini-proc function for whenever this object receives an event
	virtual void HandleEvent(IEvent* pEvent) = 0;
};

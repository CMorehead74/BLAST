/*
	File:				DestroyAllyMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Ally Message to the Message System
*/

#pragma once
#include "Message.h"

class CAlly;

class CDestroyAllyMessage : public CMessage
{
public:
	CDestroyAllyMessage(CAlly* pAlly);
	virtual ~CDestroyAllyMessage(void);

	CAlly*	GetAlly(void) const { return m_pAlly; }

private:
	CAlly*	m_pAlly;
};
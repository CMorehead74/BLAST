/*
	File:				DestroyPickUpMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy PickUp Message to the Message System
*/

#pragma once
#include "Message.h"

class CPickUp;

class CDestroyPickUpMessage : public CMessage
{
public:
	CDestroyPickUpMessage(CPickUp* pPickUp);
	virtual ~CDestroyPickUpMessage(void);

	CPickUp*	GetPickUp(void) const { return m_pPickUp; }

private:
	CPickUp*	m_pPickUp;
};
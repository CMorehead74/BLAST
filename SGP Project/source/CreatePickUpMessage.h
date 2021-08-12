/*
	File:				CreatePickUpMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create PickUp Message to the Message System
*/

#pragma once
#include "Message.h"

class CEnemy;

class CCreatePickUpMessage : public CMessage
{
public:
	CCreatePickUpMessage(CEnemy* pOwner);
	virtual ~CCreatePickUpMessage(void);

	CEnemy* GetOwner() const { return m_pOwner; }

private:
	CEnemy* m_pOwner;
};
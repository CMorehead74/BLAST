/*
	File:				CreateBulletMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create Bullet Message to the Message System
*/

#pragma once
#include "Message.h"

class CMovingObject;

class CCreateBulletMessage : public CMessage
{
public:
	CCreateBulletMessage(CMovingObject* pOwner);
	virtual ~CCreateBulletMessage(void);

	CMovingObject* GetOwner(void) const { return m_pOwner; }

private:
	CMovingObject*	m_pOwner;

};
/*
	File:				DestroyBulletMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Bullet Message to the Message System
*/

#pragma once
#include "Message.h"

class CBullet;

class CDestroyBulletMessage : public CMessage
{
public:
	CDestroyBulletMessage(CBullet* pBulletToDestroy);
	virtual ~CDestroyBulletMessage(void);

	CBullet*	GetBullet(void) const { return m_pBullet; }

private:
	CBullet* m_pBullet;
};